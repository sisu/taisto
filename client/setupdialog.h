#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include<QWidget>
#include<QDialog>
#include<QString>
#include<QLineEdit>
#include<QCheckBox>

class SetupDialog:public QDialog
{
	Q_OBJECT
	QLineEdit* nameEdit;
	QLineEdit* hostEdit;
	//QCheckBox* soundCheck;
	private slots:
	    void save();
	public:
		SetupDialog(QWidget* parent=0);
		QString name()const;
		QString hostAddress()const;
		bool sound()const;
};

#endif
