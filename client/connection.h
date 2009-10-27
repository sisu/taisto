#ifndef CONNECTION_H
#define CONNECTION_H
#include<QString>
#include<QObject>
class Connection : public QObject {
    Q_OBJECT
    public:
        Connection();
        void connect(QString);
};
#endif
