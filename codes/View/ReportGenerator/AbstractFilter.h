#ifndef ABSTRACTFILTER_H
#define ABSTRACTFILTER_H

#include <QObject>

class AbstractFilter: public QObject
{
    Q_OBJECT
public:
    	explicit AbstractFilter(QObject *parent = 0);

signals:
	void Process(int);
	void Finished(QString error = "");

public slots:
	virtual int Update() = 0;

};

#endif // ABSTRACTFILTER_H
