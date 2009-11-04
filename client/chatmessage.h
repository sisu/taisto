#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H
#include<QString>
#include<QDateTime>
struct ChatMessage {
	QDateTime timestamp;
	QString name,msg;
	ChatMessage(QDateTime t, QString n, QString m) {
		timestamp=t;
		name=n;
		msg=m;
    }
    

};

#endif
