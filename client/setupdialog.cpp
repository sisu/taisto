#include"setupdialog.h"
#include<QFormLayout>
#include<QDialogButtonBox>
#include<QSettings>
SetupDialog::SetupDialog(QWidget* parent):QDialog(parent)
{
    QSettings settings;
	QFormLayout* lay = new QFormLayout();

	setLayout(lay);
	nameEdit = new QLineEdit();
	nameEdit->setText(settings.value("name","player").toString());
	lay->addRow("Name",nameEdit);
	hostEdit = new QLineEdit();
	hostEdit->setText(settings.value("host","").toString());
	lay->addRow("Host address",hostEdit);
	//soundCheck = new QCheckBox();
	//soundCheck->setChecked(settings.value("sound",true).toBool());
	//lay->addRow("Enable sound",soundCheck);
	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
			                                      | QDialogButtonBox::Cancel);
	lay->addWidget(buttonBox);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	connect(this,SIGNAL(accepted()),this,SLOT(save()));
}
void SetupDialog::save()
{
    QSettings settings;
    settings.setValue("name",name());
    settings.setValue("host",hostAddress());
//    settings.setValue("sound",sound());
}
QString SetupDialog::name()const
{
	return nameEdit->text();
}
QString SetupDialog::hostAddress()const
{
	return hostEdit->text();
}

bool SetupDialog::sound()const
{
    return false;
//	return soundCheck->isChecked();
}


