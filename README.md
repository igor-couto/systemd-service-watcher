# Systemd Service Monitor

A quick and dirty C program that monitors specified systemd services by reading from an INI file and alerts when services are inactive.

**Systemd Service Monitor** is useful for managing servers where the services being monitored are not directly under your control. For example, it can monitor third-party service applications or personal projects that rely on external dependencies, allowing you to receive personalized alerts when these services encounter issues.

While **Systemd Service Monitor** currently logs alerts to the console, future updates may include:

- **File Logging**: Save alerts and service statuses to log files for persistent tracking and historical analysis.
- **Email Notifications**: Send automated email alerts when services become inactive, ensuring prompt awareness even when not monitoring the terminal.
- **Streaming Alerts**: Integrate with streaming platforms or dashboards for real-time monitoring and visualization.
- **Custom Actions**: Execute predefined scripts or commands in response to service status changes, enabling automated remediation steps.

## Usage

### Compile the Program
```bash
gcc -o service_monitor service_monitor.c
```
###  Run the Monitor
```bash
./service_monitor
```

# Configuration
Edit the services-to-watch.ini file to include the services you want to monitor:

```
# Service Human Readable Name=systemd-service-name
Newsletter=actions.runner.newsletter.service
systemctl status apache2.service
systemctl status docker.service
systemctl status postgresql.service
```

## Contributing

Contributions are welcome! Please open an issue or submit a pull request for any enhancements or bug fixes.

## License

This project is licensed under the GNU Affero General Public License. See the `LICENSE.md` file for details.

## Author

- **Igor Couto** - [igor.fcouto@gmail.com](mailto:igor.fcouto@gmail.com)
