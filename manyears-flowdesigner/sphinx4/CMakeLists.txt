find_package(Java)
IF(JAVA_COMPILE AND JAVA_RUNTIME)

    SET (SPHINX4TCPSERVER_FILES
        Sphinx4TcpServer.class
        js.jar
        jsapi.jar
        sphinx4.jar
        tags.jar
        TIDIGITS_8gau_13dCep_16k_40mel_130Hz_6800Hz.jar
        config.xml
        digits.gram
    )

    # make sure the class is built before calling make install
    INSTALL( CODE "EXECUTE_PROCESS( COMMAND ${JAVA_COMPILE} sphinx4/Sphinx4TcpServer.java -classpath .:sphinx4/js.jar:sphinx4/jsapi.jar:sphinx4/sphinx4.jar:sphinx4/tags.jar:sphinx4/TIDIGITS_8gau_13dCep_16k_40mel_130Hz_6800Hz.jar )" )

    install( FILES ${SPHINX4TCPSERVER_FILES} DESTINATION ${FLOWDESIGNER_TOOLBOX_DIR}/ManyEars/sphinx4 )

ENDIF(JAVA_COMPILE AND JAVA_RUNTIME)
