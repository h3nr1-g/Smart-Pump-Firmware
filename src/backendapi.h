#ifndef API_H
#define API_H


struct timings {
    unsigned long active; // in seconds
    unsigned long sleep; // in seconds
};


class BackendApi {

public:
    void setPort(unsigned short port);

    void setHost(const char *host);

    timings *fetchTimings(unsigned short id);

    void setUrlPrefix(const char *string);

    void setToken(const char *string);

    void setCertificateFingerPrint(const char *string);

private:
    const char *host = nullptr;
    unsigned short port = 443;
    const char *authToken = nullptr;
    const char *urlPrefix = nullptr;
    const char *certFingerPrint = nullptr;

    String sendRequest(unsigned short id);
};


#endif //API_H
