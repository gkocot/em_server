--------------------------WEB----------------------------
- Implement module reset from web interface;
- Place configuration in config.json;
- Parse config.json using cJSON component;
- Get configuration when App.vue mounted;
- Learn Vue state management, pass state information between Vue components;
- Indicate that cnfiguration was edited (display a message o r a button);
- Add buttons to save configuration;
- Add button to reload configuration (restart ESP modeule);


--------------------------GENERAL------------------------
- WiFi password shorter than 8 characters 'bricks' the module?!
- No mDNS on Android Chrome (that makes poor UX if someon is using smartphone for accessing the configuration server, you have to type IP http://192.168.4.1/ instead of http://em.local) https://android.stackexchange.com/questions/49188/how-to-get-mdns-working-for-chrome-on-android;
- In the example_connect() there is waiting for IP addresses, see on_got_ip(), do we need that?
- On start read softAP ssid/password from NVS, use defaults if not present;
- Allow updating softAP ssid/password from web interface (and perform module reset to apply new credentials);
- Semihosting for easier web development/testing, but this would require JTAG?

---------------------------LEARN-------------------------
- Code refactoring, nice components;
- Learn Vuex (state)
- Learn Vue router

---------------------------OTHER-------------------------
- Often after flashing app, module keeps resetting, need to power down/up the module it sometimes helps. Looks like this problem is gone;

