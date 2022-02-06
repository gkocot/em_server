idf_component_register(
    SRCS "server.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES esp_http_server json settings_provider
)
