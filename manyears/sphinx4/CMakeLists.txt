find_package(Java)
IF(JAVA_COMPILE)
    execute_process( COMMAND ${JAVA_COMPILE} sphinx4/Sphinx4TcpServer.java )

    
    SET (SPHINX4TCPSERVER_FILES
        Sphinx4TcpServer.class
        js.jar
        jsapi.jar
        sphinx4.jar
        tags.jar
        TIDIGITS_8gau_13dCep_16k_40mel_130Hz_6800Hz.jar
    )

    install( FILES SSPHINX4TCPSERVER_FILES DESTINATION ${FLOWDESIGNER_TOOLBOX_DIR}/ManyEars/sphinx4 )
ENDIF(JAVA_COMPILE)
