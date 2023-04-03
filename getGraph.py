from random import randint

class Graph:
    def __init__(self, title:str, data:list):
        self.title=title
        self.data=data
        self.body=self.setBody()
        self.urlRoot="https://static.meteociel.fr/cartes_obs/graphe.php?"
        self.url=self.urlRoot
 
    def getData(self) -> list:
        return [randint(0,30) for i in range(27)]

    def getAbsName(self) -> list[list]:
        return [[0,"Sam"],[3,"Dim"],[11,"Lun"],[19,"Mar"],[27,"Mer"]]

    def setBody(self) -> dict:
        body={
            "titre":self.title,
            "sep":1,
            "type":0,
            "data": self.getData(),
            "absname": self.getAbsName()
        }
        return body

    def setItemsData(self,d:list) -> str:
        res=""
        for i in range(len(d)):
            res+=f"data{i}={d[i]}&"
        return res

    def setItemsAbs(self,n:list) -> str:
        res=""
        for l in n:
            res+=f"absname{l[0]}={l[1]}&"
        return res

    def setUrl(self) -> None:
        self.url=self.urlRoot
        for k,i in self.body.items():
            if isinstance(i, list):
                if k=='data': 
                    self.url+=self.setItemsData(i)
                if k=='absname':
                    self.url+=self.setItemsAbs(i)
            else:
                self.url+=f"{k}={i}&"

    def getUrl(self) -> str:
        self.setUrl()
        return self.url

print(Graph("Pression",[]).getUrl())

#https://static.meteociel.fr/cartes_obs/graphe.php?titre=&sep=1&type=0&data0=12.1&data1=9.4&data2=6.7&data3=6.3&data4=7.2&data5=7.8&data6=8.7&data7=10.8&data8=11&data9=9&data10=4.8&data11=1.8&data12=2.9&data13=1.2&data14=7.8&data15=11.6&data16=12.1&data17=9.2&data18=4.4&data19=2.9&data20=1.1&data21=0.3&data22=6.4&data23=10.2&data24=11.4&data25=8.3&data26=3.8&data27=2&absname0=Sam&absname3=Dim&absname11=Lun&absname19=Mar&absname27=Mer&