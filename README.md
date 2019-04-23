# OutputTaskFile


### How To Run
Dosyaları bilgisayarınızda hangi dizinde tutuyorsanız o path'leri sırasıyla alıyoruz ve dosyaları açıyoruz.  <br/>

#### dosyayı açma işlemi
ifstream taskList;
taskList.open("tasklistPath", ios::in | ios::binary); <br/>

tasklist, taskindex ve taskfile dosyalarının dosya yollarını veriyoruz..

#### taskoutputfile dosyasını oluşturma işlemi
ofstream ofile;
ofile.open("taskoutputFilePath", ios::app);
