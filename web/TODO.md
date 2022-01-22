- Modbus/RS485 Settings page;
- Displaying charts;
- Tools to creat e mockups/designs;
- Instruction how to buiold and deploy;
- Pasting an address in browser tries to execute non existing endpoint e.g. GET /api/v1/wifi;
- Debugging Vue app;
- Library for drawing charts (Vue/JavaScript);
- RS485 Settings
- Websocket connection for live data transmission?

- Websocket connection for live data;
- Lazy loading views?
- mDNS prefix to wifi settings;
- static/dhcp IP address to wifi/network settings;
- Development API server, read/modify /conf/emconfig.json
- When settings have changed, warn if they have not been saved;

----------------SECURITY-------------------------------------------------

- We use http and when saving configuration, we pass SSID/PASSWORD!
- When AP mode selected, configuration contains unencrypted ssid/pass;
- When STA mode selected, configuration contains encrypted ssid/pass;
