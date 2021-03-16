urls=open("url.txt")
urls_new=open("urlnew.txt",'w')

urls_res=[]
num=[]


for line in urls:
    urls_res.append(line)

for i in range(1,len(urls_res)):
    j=i-1
    while(j>=0 and int(urls_res[i].split("/")[4].split("-")[0])<int(urls_res[j].split("/")[4].split("-")[0])):
        j-=1
    urls_res.insert((j+1),urls_res[i])
    del urls_res[i+1]


for i in urls_res:
    urls_new.write(i)

urls.close()
urls_new.close()