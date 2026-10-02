┌─────────────────────────────────────────────────────────────┐
│                         Application                         │
│                                                             │
│  UI Service       Logging Service       Diagnostics Service │
│  - pages          - periodic records    - UART commands     │
│  - buttons        - log formatting      - command parser    │
│  - OLED/LED       - log transport       - UART responses    │
└──────────────┬───────────────┬────────────────┬─────────────┘
               │               │                │
               ▼               ▼                ▼
┌─────────────────────────────────────────────────────────────┐
│                         Domain Services                     │
│                                                             │
│  Analytics Service              System Service              │
│  - fuel efficiency              - time and uptime            │
│  - warnings                     - logging/debug flags       │
│  - derived values               - loop statistics            │
└──────────────┬───────────────┬────────────────┬─────────────┘
               │               │                │
               ▼               ▼                ▼
┌─────────────────────────────────────────────────────────────┐
│                       Data and Hardware Services             │
│                                                             │
│  Telemetry Service              UART Transport              │
│  - sensor snapshots             - RX interrupt handling      │
│  - sensor status                - TX queue                   │
│  - sensor updates               - transmit completion        │
└──────────────┬───────────────┬────────────────┬─────────────┘
               │               │                │
               ▼               ▼                ▼
┌─────────────────────────────────────────────────────────────┐
│                       Hardware / HAL                        │
│                                                             │
│  RTC    UART    GPIO    I2C    Timers    OLED    Sensors     │
└─────────────────────────────────────────────────────────────┘

Application: 
# Module ownership
system.c
    owns:
        - current time
        - uptime
        - loop statistics
        - debug flag
        - logging enable flag
        - system events

telemetry.c
    owns:
        - TelemetryData_t
        - TelemetryStatus_t
        - sensor update state

analytics.c
    owns:
        - AnalyticsData_t
        - warning_flags_t
        - fuel-efficiency calculations

logging.c
    owns:
        - pending log event
        - log formatting
        - log scheduling

diagnostics.c / uart_transport.c
    owns:
        - UART RX state
        - UART TX queue
        - command parser state
        - command responses

ui.c
    owns:
        - current page
        - UI input state
        - display state