# OutputTaskFile


### How To Run
Dosyaları bilgisayarınızda hangi dizinde tutuyorsanız o path'leri sırasıyla alıyoruz ve dosyaları açıyoruz.  <br/>

#### Dosyayı açma işlemi
ifstream taskList;<br/>
taskList.open("**tasklistPath**", ios::in | ios::binary); <br/>

tasklist, taskindex ve taskfile dosyalarının dosya yollarını veriyoruz..

#### Taskoutputfile dosyasını oluşturma işlemi
ofstream ofile;<br/>
ofile.open("**taskoutputFilePath**", ios::app);
