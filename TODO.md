- rest_common_get_handler is not correct, if uri is wrong, it tries fetch not existing files!
- Move to C++17;

--------------------------GENERAL------------------------

- No mDNS on Android Chrome (that makes poor UX if someon is using smartphone for accessing the configuration server, you have to type IP http://192.168.4.1/ instead of http://em.local) https://android.stackexchange.com/questions/49188/how-to-get-mdns-working-for-chrome-on-android;
- In the example_connect() there is waiting for IP addresses, see on_got_ip(), do we need that?
- Semihosting for easier web development/testing, but this would require JTAG?

---------------------------LEARN-------------------------

- Code refactoring, nice components;
- Learn Vuex (state)
- Learn Vue router

---------------------------OTHER-------------------------

- Often after flashing app, module keeps resetting, need to power down/up the module it sometimes helps. Looks like this problem is gone;
