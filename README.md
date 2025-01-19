# Systemd Service Monitor

A C program that monitors specified systemd services by reading from an INI file and alerts when services are inactive. When any of the monitored services become inactive, the tool promptly alerts the user via the terminal.

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
