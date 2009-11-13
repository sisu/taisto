#ifndef STATS_H
#define STATS_H
#include<QtAlgorithms>
#include<QList>
#include<QString>
struct StatsPlayer {
    StatsPlayer(int _id,QString _name, int _kills, int _deaths, double _damage) {
        id=_id;
        name=_name;
        kills=_kills;
        deaths=_deaths;
        damage=_damage;
    }
    int id;
    QString name;
    int kills;
    int deaths;
    double damage;
    bool operator<(const StatsPlayer& b) const {
        if(deaths==0&&b.deaths==0) return kills>b.kills;
        else if(deaths==0) return true;
        else if(b.deaths==0) return false;
//        else return kills*1.0/deaths>b.kills*1.0/deaths;
		return double(kills)*b.deaths > double(b.kills)*deaths;
    }


};

struct Stats {
    QList<StatsPlayer> players;
    QList<QString> tostring;
    Stats() {
        players.clear();
        players.append(StatsPlayer(0,"Siika",2,4,5.0));
        players.append(StatsPlayer(0,"Siika",2,4,5.0));
        makestring();       
    }
    void makestring() {
        sort();
        //Make string
		tostring.clear();
        for(int i=0;i<players.size();i++) {
            QString str="";
            StatsPlayer& p=players[i];
//            str+=QString::number(p.id);
//            str+="\t";
            str+=p.name;
            str+="\t";
            str+=QString::number(p.kills);
            str+="\t";
            str+=QString::number(p.deaths);
            str+="\t";
            if(p.deaths>0)  str+=QString::number(p.kills*1.0/p.deaths);
            else str+="inf";
//            str+="\t";
//            str+=QString::number(p.damage);
            tostring.append(str);
        }

    }
    void sort() {
        if(!players.empty()) qSort(players.begin(),players.end());
    }


};


#endif // STATS_H
