


-----------İBRAHİM AKBULUT----------------
  -----------151044077-----------------



SPIMOS_GTU s dosyaları spim ile çalıştırılır.

SPIMOS_GTU_1.s dosyası için:

init sistem çağrısı yapılır, bu çağrı kaç process olacağını söyler ve o kapasitede bir process table oluşturur. Process table'in ilk elemanı default olarak 0 id'sine sahip init process olur. İnit process'i SPIM_OS_GTU_1.s dosyasının yürüteceği işlemler olarak ele aldım.
Daha sonra 3 tane fork sistem çağrısı yapılır. Bu çağrı init processi kopyalar ve tabloya doldurur. Daha sonra ise 3 tane execv sistem çağrısı yapılır. Bu çağrı parametre olarak memorye yükleyeceği programın ismini ve hangi process üzerinden yürüyecekse onun id'sini alır.
Tabloda o process id'sine sahip processin bilgilerinin yerine hedef programı kaydeder. Daha sonra sonsuz bir döngü içinde wait_pid sistem çağrısı yapılır. Bu çağrı Process table daki tüm processlerin bitmesi durumunda exit(1) ile tüm programı sonlandırır(SPIMOS_GTU_1 sonlandırılır yani). 

SPIMOS_GTU_2.s dosyası için:

Bu yöntemde random_generator adlı sistem çağrısı yapılır ve rastgele bir sayı elde edilir. Bu sayı 3 program arasında hangi programın memory'e 10 defa yükleneceğini belirler. Diğer adımlar 1 deki gibidir.

SPIMOS_GTU_3.s dosyası için:

Bundada aynı şekilde rastgele ve birbirinden farklı 2 sayı üretilir ve bu sayılara göre iki program memorye 3'er defa yüklenir. Diğer adımlar 1 deki gibidir.


BinarySearch,LinearSearch ve Collactz programlarının sonunda process_exit sistem çağrısı yapılır bu çağrı kendisinin sonlandığı process table daki durum flagini değiştirerek bildirir. 


ContextSwitch işlemleri bir fonksiyon altında yazılmıştır ve bu fonksiyon yalnızca SPIM_timerhandler tarafından çağrılmaktadır. Yani yalnızca timer interrupt geldiğinde context switch yapılır.


Fork sistem çağrısı: Hocam bu çağrıyı implement ederken aklıma gelen şuydu; Unixde tüm processler init process üzerinde türeyerek işler diye hatırlıyorum o yüzden bu çağrıda init processi kopyaladım yani bir nevi init processin kopyasını yaratmış oldum. Daha sonra bu kopya execv ile kendisine atanacak(memory'si değişecek) görevi yerine getirir diye düşündüm. Bunu farklı yapanlar oldu onuda biliyorum ama dediğim gibi bana mantıklı gelen bu idi.

Not: BinarySearch ve diğer test programlarının sonunda process_exit sistem çağrısından sonra boş ve sonsuz bir loop kullandım. Eğer sonsuz bir döngü en sonda olmaz ise context switch çağrısını process_exit sistem çağrısı içinde yapmak zorunda kalıyordum. Ödev pdf'inde contextswitch'in yalnızca timer ile gerçekleceği yazılmış. Yani o sonsuz loop sadece timeri bekliyor tek görevi o. Bunuda process_exit sistem çağrısın bir bütünü olarak düşündüm.

