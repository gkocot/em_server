- Lazy loading views?
- mDNS prefix to wifi settings;
- static/dhcp IP address to wifi/network settings;
- Development API server, read/modify /conf/emconfig.json
- When settings have changed, warn if they have not been saved;

----------------SECURITY-------------------------------------------------

- We use http and when saving configuration, we pass SSID/PASSWORD!
- When AP mode selected, configuration contains unencrypted ssid/pass;
- When STA mode selected, configuration contains encrypted ssid/pass;
