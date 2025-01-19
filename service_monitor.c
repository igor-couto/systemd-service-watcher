#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_SERVICES    100
#define MAX_LINE_LENGTH 256
#define CHECK_INTERVAL_IN_SECONDS 60

typedef struct {
    char human_readable_name[MAX_LINE_LENGTH];
    char service_name[MAX_LINE_LENGTH];
} ServiceInfo;

int load_services(const char *filename, ServiceInfo *services, int max_services) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: Could not open %s\n", filename);
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    int service_count = 0;

    while (fgets(line, sizeof(line), fp)) {
        // Remove trailing newline
        line[strcspn(line, "\r\n")] = '\0';

        // Skip empty lines or those starting with '#' or ';'
        if (line[0] == '\0' || line[0] == '#' || line[0] == ';') {
            continue;
        }

        char *equals_pos = strchr(line, '=');
        if (!equals_pos) {
            // No '=' found, skip this line or handle error
            fprintf(stderr, "Warning: Invalid line (no '='): %s\n", line);
            continue;
        }

        // Split into two parts: human-readable name = systemd service name
        *equals_pos = '\0'; // Replace '=' with null terminator

        char *human_readable_name = line;
        char *service_name = equals_pos + 1;

        // Trim leading/trailing spaces on human_readable_name
        while (isspace((unsigned char)*human_readable_name)) {
            human_readable_name++;
        }
        
        char *end = human_readable_name + strlen(human_readable_name) - 1;
        while (end > human_readable_name && isspace((unsigned char)*end)) {
            *end = '\0';
            end--;
        }

        // Trim leading/trailing spaces on service_name
        while (isspace((unsigned char)*service_name)) service_name++;
        end = service_name + strlen(service_name) - 1;
        while (end > service_name && isspace((unsigned char)*end)) {
            *end = '\0';
            end--;
        }

        if (service_count < max_services) {
            strncpy(
                services[service_count].human_readable_name, 
                human_readable_name, sizeof(services[service_count].human_readable_name) - 1
            );
            strncpy(
                services[service_count].service_name, 
                service_name,
                sizeof(services[service_count].service_name) - 1
            );

            // Ensure null termination
            services[service_count].human_readable_name[sizeof(services[service_count].human_readable_name) - 1] = '\0';
            services[service_count].service_name[sizeof(services[service_count].service_name) - 1] = '\0';
            service_count++;
        } 
        else {
            fprintf(stderr, "Warning: Too many services, max is %d\n", max_services);
            break;
        }
    }

    fclose(fp);

    return service_count;
}

void monitor_services(const ServiceInfo *services, int service_count, unsigned int interval) {
    while (1) {
        for (int i = 0; i < service_count; i++) {
            char cmd[512];
            snprintf(cmd, sizeof(cmd), "systemctl is-active %s", services[i].service_name);

            // Run the systemctl command
            FILE *cmd_fp = popen(cmd, "r");
            if (!cmd_fp) {
                fprintf(stderr, "Failed to run command: %s\n", cmd);
                continue;
            }

            char result[128] = {0};
            if (fgets(result, sizeof(result), cmd_fp)) {
                // Remove trailing newline
                result[strcspn(result, "\r\n")] = '\0';

                if (strcmp(result, "active") != 0) {
                    printf("WARNING: '%s' is NOT running (status: '%s')\n", services[i].human_readable_name, result);
                }
            }

            pclose(cmd_fp);
        }

        sleep(interval);
    }
}

int main(void) {
    ServiceInfo services[MAX_SERVICES];
    int service_count = load_services("services-to-watch.ini", services, MAX_SERVICES);

    if (service_count < 1) {
        fprintf(stderr, "No services loaded or error occurred. Exiting.\n");
        return 1;
    }

    monitor_services(services, service_count, CHECK_INTERVAL_IN_SECONDS);

    return 0;
}
