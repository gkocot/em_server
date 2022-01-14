- No mDNS on Android Chrome (that makes poor UX if someon is using smartphone for accessing the configuration server, you have to type IP http://192.168.4.1/ instead of http://em.local) https://android.stackexchange.com/questions/49188/how-to-get-mdns-working-for-chrome-on-android;
- Often after flashing app, module keeps resetting, need to power down/up the module it sometimes helps;
- After changing to softAP mode only, the frontend cannot acces stylesshets from the internet, pages look bad, don't use resources from  the Internet;
- In the example_connect() there is waiting for IP addresses, see on_got_ip(), do we need that?
- On start read softAP ssid/password from NVS, use defaults if not present;
- Implement module reset from web interface;
- Allow updating softAP ssid/password from web interface (and perform module reset to apply new credentials);

- Vuex (state)
- Vue router

