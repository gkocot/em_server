- Implement POST /api/v1/modbus handler in backend (On ESP32);
- Modify endpoints to only apply changes for modbus or wifi (don't work with the whole configuration, some part is more like capabilities);
- nodemon for the development server, so it sees changes in backend API, configuration etc.
- Backend API documentation, preferably autogenerated from server/api.json;

- Displaying charts, find a library for drawing charts (Vue/JavaScript);
- Tools to creat e mockups/designs;
- Instruction how to build and deploy the configuration web page;
- Websocket connection for live data transmission?
- Localization, and app settings page (selecting language, light/dark mode etc.);

- Lazy loading views?
- mDNS prefix to wifi settings;
- static/dhcp IP address to wifi/network settings;
- When settings have changed, warn if they have not been saved;

----------------SECURITY-------------------------------------------------

- We use http and when saving configuration, we pass SSID/PASSWORD!
- When AP mode selected, configuration contains unencrypted ssid/pass;
- When STA mode selected, configuration contains encrypted ssid/pass;
