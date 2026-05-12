
CÔNG TY TNHH KỸ THUẬT QUẢN LÝ BAY
ATTECH ISO 9001:2015
========= & =========






THUYẾT MINH THIẾT KẾ



TÊN SẢN PHẨM: HỆ THỐNG ĐỒNG HỒ THỜI GIAN CHUẨN GPS
MÃ SẢN PHẨM:  
CƠ QUAN CHỦ TRÌ:  PHÒNG NGHIÊN CỨU PHÁT TRIỂN
  



           PHÊ DUYỆT           PHÒNG NCPT         NGƯỜI LẬP









Hà Nội, tháng    năm 202

MỤC LỤC

1.	Căn cứ thiết kế	3
2.	Yêu cầu thiết kế	4
2.1.	Công năng của sản phẩm	4
2.2.	Yêu cầu về lắp đặt, khai thác, bảo trì sản phẩm	4
2.3.	Yêu cầu về công nghệ chế tạo sản phẩm	4
3.	Các thông số và đặc tính kỹ thuật của sản phẩm	5
3.1	Đặc tính kỹ thuật chung của hệ thống	5
3.2	Các đặc tính kỹ thuật thiết bị đồng hồ Master	5
3.3	Đặc tính kỹ thuật đồng hồ Slave	6
3.4	Các tính năng cải tiến và bổ sung	7
4.	Giải pháp thiết kế	8
4.1.	Lựa chọn giải pháp công nghệ	8
4.2.	Giải pháp thiết kế	9
4.2.1 Giải pháp thiết kế đồng hồ master	9
4.2.1.1 Sơ đồ khối đồng hồ master – slave	9
4.2.1.2 Giải pháp thiết kế chi tiết đồng hồ master	10
a)	Giải pháp thiết kế mạch GPS	11
b)	Giải pháp thiết kế mạch main	13
c)	Giải pháp thiết kế các bộ chuyển đổi nguồn đầu vào đồng hồ master:	22
d)	Giải pháp thiết kế mạch giao tiếp	23
4.2.2 Giải pháp thiết kế đồng hồ Slave treo tường dùng LED 7 thanh	31
a) Sơ đồ khối đồng hồ Slave	31
b) Giải pháp thiết kế mạch điều khiển slave	31
c) Giải pháp thiết kế mạch hiển thị LED	32
d) Giải pháp thiết kết phần mềm nhúng đồng hồ slave	32
4.2.3 Giải pháp thiết kế đồng hồ Slave để bàn console dùng LED 7 thanh	34
4.2.4 Giải pháp thiết kế đồng hồ Slave treo tường dùng LED ma trận	35
4.2.5.	Giải pháp thiết kế vỏ thiết bị	35
5.	Giải pháp công nghệ chế tạo	35
6.	Quy trình và thiết bị công nghệ sử dụng chế tạo sản phẩm	36
7.	Tiêu chí lựa chọn nhà cung ứng sản phẩm dịch vụ để mua linh kiện, thiết bị	36



































1.	Căn cứ thiết kế
- Căn cứ quyết định số 371/QĐ-HĐQLQ ngày 3/8/2023 của CTCT về việc phê duyệt phương án chuyển giao sản xuất nhiệm vụ KHCN "Cải tiến đồng hồ GPS để nâng cao tính ổn định và thuận tiện cho khai thác lắp đặt".
- Căn cứ hồ sơ chuyển giao sản xuất đồng hồ GPS (hồ sơ có mã HSSX-02-2024-V02-B)
- Căn cứ chỉ đạo của Giám đốc tại cuộc họp ngày 02/12/2025 liên quan đến cung cấp sản phẩm đồng hồ GPS master có 3 cổng NTP cho khách hàng, phòng NCPT đã rà soát lại toàn bộ hồ sơ thiết kế và thực hiện lập bản vẽ thiết kế và tiên lượng mạch giao tiếp đồng hồ GPS master đáp ứng yêu cầu của khách hàng.
- Căn cứ biên bản 58/BB-KTATCL về việc thẩm định nội dung hiệu chỉnh HSTK sản phẩm hệ thống đồng hồ thời gian chuẩn GPS (phần điện) ngày 09/4/2026.
- Hướng dẫn thiết kế mạch điện tử đã được sửa đổi và ban hành có hiệu lực từ ngày 10/2/2026 (Ký hiệu: HD-TKMĐT/Lần BH:02/Ngày HL: 10/2/2026)
- Các tài liệu của Hệ thống quản lý chất lượng theo tiêu chuẩn ISO 9001:2015.
2.	Yêu cầu thiết kế
2.1.	 Công năng của sản phẩm
-	Hệ thống đồng hồ thời gian chuẩn GPS bao gồm thiết bị master nhận dữ liệu thời gian chuẩn từ GPS phục vụ đồng bộ thời gian cho các thiết bị đầu cuối bao gồm các đồng hồ slave phục vụ hiển thị và các hệ thống khác như hệ thông ghi âm, hệ thống AMHS và các hệ thống máy tính khác.  
-	 Các đồng hồ slave sử dụng LED 7 thanh hiển thị thông tin gồm ngày, tháng, năm, giờ, phút, giây và thời gian được cập nhật theo từng giây.
-	Hệ thống đồng hồ thời gian chuẩn GPS có thể được lắp đặt tại các sân bay để cung cấp thời gian chuẩn phục vụ hoạt động quản lý bay bao gồm công tác điều hành không lưu, đồng bộ cho các hệ thống ghi dữ liệu và thoại chuyên dụng. Bên cạnh đó, hệ thống đồng hồ thời gian chuẩn GPS có thể được ứng dụng cho các hệ thống quân sự và dân sự. 	
2.2.	 Yêu cầu về lắp đặt, khai thác, bảo trì sản phẩm
-	Đối với thiết bị đồng hồ master:
•	 Thiết bị đồng hồ master lắp đặt phù hợp trên Rack thiết bị chuẩn 19 inch.
•	 Thiết bị master cần đáp ứng yêu cầu về độ chính xác, tin cậy, tính ổn định và thuận tiện trong công tác bảo dưỡng, sửa chữa; có các đầu ra cung cấp cho đồng hồ slave, thiết bị máy tính (thông qua NTP) và các thiết bị khác theo chuẩn IRIG-B.
-	Đối với đồng hồ hiển thị:
•	 Đồng hồ hiển thị Slave có 2 loại: loại treo tường và loại lắp trên bàn Console.
•	 Các đồng hồ hiển thị có các số hiển thị rõ ràng, kiểu dáng công nghiệp, đảm bảo mỹ thuật, dễ lắp đặt, độ tin cậy cao, hoạt động tốt trong môi trường có nhiệt độ từ 0 oC đến 50oC, độ ẩm đến 95%.
2.3.	 Yêu cầu về công nghệ chế tạo sản phẩm
-	Thiết bị được thiết kế trên cơ sở ứng dụng các công nghệ mới, hiện đại nhất, phù hợp điều kiện sản xuất trong nước.
-	Các thiết bị, linh kiện điện tử phải có độ tin cậy, ổn định cao, nguồn vật tư sẵn có trên thị trường.
-	Kết cấu khung vỏ phải đảm bảo độ cứng vững, an toàn trong suốt quá trình lắp đặt, khai thác và vận hành.
-	Các vật tư lắp đặt để liên kết các chi tiết với nhau phải sử dụng các vật tư tiêu chuẩn như: gioăng cao su, bu lông, đệm phẳng, đai ốc, vít.
3.	Các thông số và đặc tính kỹ thuật của sản phẩm
3.1	Đặc tính kỹ thuật chung của hệ thống
Hệ thống gồm một đồng hồ master thu tín hiệu từ vệ tinh rồi cung cấp tín hiệu có độ chính xác cao tới các đồng hồ slave để hiển thị thông tin về thời gian đồng thời gửi thông tin đồng bộ thời gian cho các thiết bị khác như hệ thống ghi thoại và dữ liệu, hệ thống AMHS và hệ thống thiết bị CNTT khác…
Đồng hồ master gồm hai anten đặt tại hai vị trí khác nhau nối với hai khối thu nằm trong đồng hồ master. Hai khối thu này có chức năng hoạt động song song và dự phòng cho nhau để đảm bảo tính liên tục của dữ liệu. Trạng thái các khối thu và thời gian thu được của đồng hồ master được hiển thị trên màn hình LCD.
Khi mất toàn bộ tín hiệu GPS, các thiết bị slave sẽ được đồng bộ theo tín hiệu thời gian từ đồng hồ master. Trường hợp không có tín hiệu đồng bộ từ đồng hồ master, các đồng hồ slave sẽ hiển thị thời gian theo đồng hồ nội của slave.
Nguồn điện sử dụng cho sản phẩm là nguồn xoay chiều 220VAC/50Hz hoặc nguồn một chiều 24:48VDC tùy theo yêu cầu.
3.2	Các đặc tính kỹ thuật thiết bị đồng hồ Master
-	Nguồn điện cung cấp: đồng hồ GPS Master có thể sử dụng nguồn điện 220VAC hoặc 24:48VDC. Các nguồn điện này có thể sử dụng kết hợp tùy theo yêu cầu khai thác gồm: 02 nguồn AC, 01 nguồn AC kết hợp 01 nguồn DC.
•	 Thông số điện của nguồn vào AC:  220VAC ± 10%, 50Hz;
•	 Thông số điện của nguồn vào DC:  24 ÷ 48 VDC ±10%.
-	Kích thước vật lý (Không tính tai cài): 430 x 214 x 44.45 (mm), lắp đặt phù hợp trên rack 19’’.
-	Hai bộ thu GPS hoạt động song song và dự phòng cho nhau.
-	Số anten GPS: 02
-	Chiều dài dây phi-đơ anten: cung cấp theo yêu cầu của khách hàng với khoảng cách lên tới 300m.
-	Môi trường hoạt động:
•	 Nhiệt độ: 0 oC -50oC
•	 Độ ẩm: đến 95%.
-	Đặc tính bộ thu tín hiệu GPS:
•	 Độ nhạy: -161 dBm.
•	 Độ chính xác khi có đồng bộ GPS: < 400 ns.
•	 Độ chính xác khi không có đồng bộ GPS: < 0.1s/ngày.
-	Sai số thời gian hiển thị ≤ 100ms so với thời gian thu được của các máy thu GPS.
-	Màn hình LCD hiển thị trạng thái các khối thu GPS và thời gian thu được.
- Thông tin về địa chỉ IP của thiết bị
-	Tín hiệu đầu ra bao gồm RS-485 và IRIG-B.
-	Dữ liệu đầu ra đồng bộ thời gian cho các thiết bị khác:
•	 06 đầu ra RS-485, tốc độ 9600 Kbps, đồng bộ cho các đồng hồ slave. Các tín hiệu đầu ra có tính chất độc lập, khi một đầu ra có sự cố sẽ không gây ảnh hưởng đến các đầu ra khác. Mỗi cổng hỗ trợ đồng bộ tối đa 32 đồng hồ slave. 
•	 02 đầu ra IRIG-B xoay chiều có cách ly bảo vệ đầu ra thông qua biến áp cách ly và có thể điều chỉnh biên độ tín hiệu 10Vp-p (Peak to peak), có thể mở rộng theo yêu cầu.
•	 03 đầu ra ethernet RJ-45 phục vụ đồng bộ NTP và SNMP.
•	 01 đầu ra RS232
- Đặc tính NTP.
•	NTP version 4
•	Đồng bộ thời gian cho các thiết bị máy tính qua giao thức NTP.
- Đặc tính SNMP, SNMP version 1: Cung cấp thông tin cho các máy tính kết nối với thiết bị master qua mạng LAN bao gồm: trạng thái tín hiệu của chip thu GPS1, chip thu GPS2, trạng thái của thiết bị master, trạng thái các module nguồn của thiết bị master.
3.3	Đặc tính kỹ thuật đồng hồ Slave 
	Có 03 loại đồng hồ slave:
-	Đồng hồ slave treo tường sử dụng Led 7 thanh
-	Đồng hồ slave treo tường sử dụng Led ma trận
(2 loại này đều có thể lắp trên rack 19inch thông qua tai bắt rack)
-	Đồng hồ slave lắp bàn console sử dụng led 7 thanh
-	Kích thước vật lý:
-	 (Không tính tai cài): 320x150x60 (mm) đối với đồng hồ treo tường.
-	 (Không tính tai cài): 152x79x63(mm) đối với đồng hồ lắp bàn console
-	Thông tin về thời gian được hiển thị trên LED 7 thanh hoặc LED ma trận và được cập nhật 1 lần/giây.
-	Độ chính xác bộ dao động nội nhỏ hơn 0.65 s/ngày.
-	Có thể chỉnh được độ sáng của LED.
-	Có đồng bộ qua NTP, RS485
-	Nguồn điện cung cấp: nguồn xoay chiều 220V ±10%, 50Hz
3.4	Các tính năng cải tiến và bổ sung so với hồ sơ có mã HSSX-02-2024-V02-B
•	Mở rộng và nâng cấp cổng giao tiếp mạng (NTP): Nâng cấp đột phá từ 01 cổng lên 03 cổng RJ45 độc lập cung cấp chức năng NTP time server. Để đảm bảo hiệu năng hoạt động tối đa cho giao thức NTP, thiết bị được thiết kế tăng cường thêm khối xử lý, tổng cộng sử dụng 03 vi xử lý STM32 và 03 IC mạng W5500 riêng biệt cho 3 cổng. Tích hợp webserver cho phép người dùng cấu hình 03 địa chỉ IP độc lập lưu trên IC nhớ.
•	Bổ sung bộ thu GPS: thêm lựa chọn sử dụng chip thu NEO-F9P-15* của hãng U-blox, giúp thêm lựa chọn bộ thu GPS, không chỉ khắc phục khó khăn về nguồn cung linh kiện cũ mà còn giúp chủ động chế tạo, nâng cao tính ổn định.
•	Cải tiến chuẩn hóa các thành phần nguồn: Tối ưu hóa cấu hình nguồn đầu vào với vài phương án nguồn AC kết hợp 01 nguồn DC dự phòng. Thử nghiệm sử dụng các module nguồn DC-DC chuyên dụng phòng khi các linh kiện hiện tại hết hàng (như PRD15-Q48-S12-N-D, K7805M, K783V3M) khắc phục khó khăn về nguồn cung linh kiện cũ mà còn giúp chủ động chế tạo, nâng cao tính ổn định.
4.	Giải pháp thiết kế
4.1.	 Lựa chọn giải pháp công nghệ
a)	Giải pháp thu tín hiệu thời gian chuẩn
Sử dụng chip thu GPS có độ nhạy cao, sẵn có trên thị trường đáp ứng được yêu cầu sản xuất hệ thống đồng hồ thời gian chuẩn của Công ty.
	Chip vi điều khiển cấu hình mạnh được sử dụng làm bộ xử lý trung tâm, đây là vi điều khiển có đầy đủ các module chức năng ADC, giao tiếp I2C, và các module điều chỉnh độ rộng xung PWM (Pulse Width Modulation), bộ DAC (digital analog converter).
b)	 Giải pháp đồng bộ cho đồng hồ slave
Sau khi thu nhận thời gian chuẩn từ các bộ thu GPS, chíp vi xử lý sẽ đọc các dữ liệu thu được để có được dữ liệu về thời gian GPS. Sau đó tín hiệu được đưa song song tới các đồng hồ slave qua mạng RS485, NTP. Các đồng hồ Slave sẽ nhận được dữ liệu về thời gian từ đồng hồ master để hiển thị các thông tin về thời gian theo từng loại đồng hồ.
c)	 Giải pháp tạo tín hiệu và điều chỉnh biên độ đầu ra IRIG-B
Sau khi nhận được dữ liệu thời gian, dữ liệu sẽ được chíp vi xử lý điều chế tín hiệu theo đúng chuẩn IRIG-B thông qua chắc năng DAC của vi điều khiển. Tín hiệu được đưa tới tầng đệm và được đưa vào tầng khuyếch đại tín hiệu sử dụng IC khuếch đại thuật toán và biến áp cách ly. Sử dụng biến trở tinh chỉnh để điều chỉnh biên độ đầu ra IRIG-B. Biến trở được thiết kế lắp tiện dụng để người dùng có thể dễ dàng điều chỉnh từng đầu ra IRIG-B tương ứng mà không cần tháo lắp vỏ thiết bị.
d)	Giải pháp thiết kế module giao tiếp NTP-SNMP
Sử dụng IC chuyên dụng để giao tiếp với mạng ethernet. Các tính năng NTP-SNMP được xử lý riêng biệt bằng vi xử lý riêng độc lập. Bản tin thời gian từ mạch main sẽ gửi tới mạch NTP-SNMP. Vi xử lý chuyển đổi và xử lý bản tin này.
Đối với phần giao tiếp mạng qua cổng RJ45, sử dụng IC chuyên dụng có chức năng giao tiếp mạng qua cổng RJ45 và giao tiếp với vi xử lý qua SPI từ đó.
e)	Giải pháp thiết kế vỏ đồng hồ
Vỏ hộp được thiết kế bằng phần mềm chuyên dụng, chế tạo bằng công nghệ CNC và một số công nghệ gia công cơ khí khác tương tự với hệ thống đồng hồ cũ. Các phần tử cấu thành thiết bị này cần đáp ứng yêu cầu hoạt động liên tục, ổn định trong môi trường có nhiệt độ từ 0oC -50oC, độ ẩm lên đến 90%.
Sử dụng công nghệ CNC, hàn và gia công cơ khí, sơn tĩnh điện để thực hiện chế tạo sản phẩm.
4.2.	 Giải pháp thiết kế 
4.2.1 Giải pháp thiết kế đồng hồ master	
4.2.1.1 Sơ đồ khối đồng hồ master – slave
	Sơ đồ khối hệ thống đồng hồ GPS master – slave
 
Hình 1: Sơ đồ đấu dây hệ thống đồng hồ thời gian chuẩn GPS

Mô tả hoạt động của hệ thống:
Hệ thống đồng hồ GPS bao gồm 01 đồng hồ GPS master và nhiều đồng hồ slave. Dữ liệu thời gian chuẩn được lấy từ tín hiệu GPS qua các bộ thu GPS chuyên dụng. Vi xử lý của đồng hồ master sẽ bóc tách dữ liệu thời gian và chuyển thành các dữ liệu thời gian theo các chuẩn: NTP server, IRIGB, RS485, RS232 để đồng bộ cho các thiết bị nối tới đồng hồ master như đồng hồ slave, máy tính. Các đồng hồ slave được trang bị hiển thị thời gian bằng LED 7 thanh hoặc LED ma trận với màn hình hiển thị lớn, dễ nhìn. Các đồng hồ slave có các loại: treo tường hoặc lắp bàn console. Các đồng hồ slave có thể đồng bộ thời gian qua NTP client hoặc RS485 với đồng hồ master.  


4.2.1.2 Giải pháp thiết kế chi tiết đồng hồ master

  
Hình 2: Sơ đồ khối đồng hồ master

Mô tả hoạt động của thiết bị đồng hồ GPS master:
Thời gian chuẩn được bóc tách từ tín hiệu GPS, đồng hồ được trang bị 02 bộ thu GPS để một bộ chạy chính và một bộ dự phòng. Tín hiệu GPS được thu qua 02 antenna GPS để vào 02 bộ thu độc lập. Khối vi xử lý tách tín hiệu thời gian chuẩn và chuyển đổi thành các dạng thời gian để cung cấp cho các thiết bị khác qua các chuẩn NTP server, RS485, RS232, IRIG-B. Để thuận tiện thiết kế, các mạch của đồng hồ GPS master được phân chia như sau:
-	Mạch GPS: Khối thu GPS có 02 chip thu GPS chạy độc lập và các thành phần bảo vệ các chip thu GPS.
-	Mạch main: bao gồm khối vi xử lý STM32, khối đồng hồ thời gian thực, khối nguồn vi xử lý
-	Nguồn vào: Đồng hồ được trang bị 2 bộ nguồn độc lập AC-AC hoặc AC-DC theo yêu cầu của khách hàng. Tùy phiên bản do khách hàng yêu cầu mà đầu vào nguồn có thể là điện AC 220V hoặc DC 48V. Khối nguồn luôn được dự phòng 2 nguồn độc lập nhau dự phòng cho nhau.
-	Mạch giao tiếp: khối NTP-SNMP, khối RS485, khối IRIG-B, khối RS232.


a)	Giải pháp thiết kế mạch GPS
 
Hình 3: Sơ đồ khối mạch thu tín hiệu GPS
Mô tả hoạt động mạch thu tín hiệu GPS:
Mạch thu tín hiệu GPS có đầu vào là nguồn 5V từ mạch nguồn cấp tới. Nguồn này sẽ chuyển thành 2 nguồn 3.3V độc lập để cấp riêng cho từng bộ thu GPS1 và GPS2. Mỗi bộ thu có một antenna độc lập. Tín hiệu thời gian sẽ được chuyển sang mạch main để xử lý. Ngoài ra mạch còn khối thời gian thực để lưu trữ và giữ xung nhịp thời gian chuẩn để cung cấp thời gian cho vi xử lý khi cần thiết. Thời gian được lưu trữ trên khối thời gian thực kể cả khi mất điện.
Chức năng, yêu cầu:
-	Thu thông tin từ vệ tinh qua antenna nối vào các bộ thu GPS chuyên dụng
-	Có 02 bộ thu GPS hoạt động độc lập
-	Có các khối nguồn độc lập cho mỗi bộ thu GPS
-	Độ nhạy: -161dBm
-	Độ chính xác: 400ns
Đầu vào: tín hiệu GPS từ antenna thu GPS.
Đầu ra: bản tin thời gian được xuất ra các cổng UART của các module thu GPS. Truyền dữ liệu thời gian qua chân TX của uart
Giải pháp thiết kế:
Mạch GPS được thiết kế sử dụng 02 bộ thu GPS độc lập, bộ cấp nguồn độc lập cho mỗi chip thu GPS. LEA-M8F là dòng chip GPS chuyên dụng về thời gian và tần số của hãng U-blox. Tín hiệu GPS sẽ được thu từ antenna GPS và tới thẳng bộ thu. Bộ thu sẽ bóc tách thành các dữ liệu thời gian, vị trí và gửi ra qua các chân TX của mỗi bộ thu. Mạch thu GPS được thiết kế dưới dạng module lắp trên mạch main để tiện thay thế, nâng cấp bộ thu GPS
 
Hình 4: Sơ đồ nguyên lý chip thu GPS
Thông số chính của chip LEA-M8F:
LEA-M8F có các giao tiếp vật lý: UART, USB, SPI, I2C. Truyền thông bản tin theo các chuẩn NMEA 0183 V4.0, UBX, RTCM 2.3. Trong thiết kế sản phẩm lựa chọn giao tiếp UART. Các bản tin ra gồm có GGA, GLL, GSA, GSV, RMC, VTG, TXT, GPZDA…. Thông tin về thời gian được lấy trong bản tin GPZDA, thông tin chi tiết được nêu trong mục thiết kế phần mềm nhúng.
Độ nhạy tối đa: -165 dBm
Độ chính xác thời gian < 20ns
b)	Giải pháp thiết kế mạch main
Mạch main nhận thông tin thời gian từ các bộ thu GPS và chuyển thành thời gian thực. Các thông tin hoạt động, cấu hình, trạng thái được báo trên màn hình LCD 40x2. Màn hình LCD và mạch hiển thị trạng thái hoạt động của đồng hồ. Màn hình LCD dùng để hiển thị trạng thái GPS, thời gian và IP của thiết bị đồng hồ. Các đèn chỉ thị trạng thái đồng hồ gồm có: nguồn, GPS1, GPS2, GPS.

  
Hình 5: Sơ đồ khối mạch main trong đồng hồ master

Chức năng, yêu cầu:
-	Xử lý thông tin về dữ liệu thời gian GPS từ ngõ vào RX của UART;
-	Bóc tách thời gian chuẩn từ các bản tin GPS
-	Chuyển đổi thời gian chuẩn thành các bản tin thời gian qua cổng UART TTL cho mạch giao tiếp theo định dạng DDMMYYHHMMSS.
Đầu vào: 
-	02 dữ liệu GPS từ 02 bộ thu GPS cung cấp bản tin thời gian từ GPS, loại bản tin NEMA là $GPRMC
Đầu ra: 
	- Hiển thị thển thị trạng thái hoạt động trên màn hình LCD 40x2
- Hiển thị trạng thái hoạt động lên các LED
- Tín hiệu thời gian qua cổng UART TTL tới mạch giao tiếp.
Như vậy, vi điều khiển được sử dụng cần thỏa mãn các yêu cầu ít nhất: có 02 cổng UART. Có thể lựa chọn các dòng vi điều khiển AVR, PIC, ARM,… để sử dụng. Qua xem xét, lựa chọn sử dụng vi điều khiển STM32G474RBT6, do dòng chip này đáp ứng các yêu cầu kỹ thuật và được phát triển, ứng dụng rộng rãi trong nhiều lĩnh vực công nghiệp cũng như dân dụng, đây là dòng chip mới, hiệu năng cao của hãng STM. Đồng thời, phòng thí nghiệm kỹ thuật điện tử đã được trang bị các công cụ thiết kế, phát triển phần cứng, phần mềm nhúng cho dòng vi điều khiển này.
 
Hình 6: Sơ đồ nguyên lý mạch vi xử lý STM32

Các thông số cơ bản của vi xử lý 32-bit STM32G474RBT6 như sau:
- Tần số hoạt động tối đa lên tới 107MHz;
- Bộ nhớ Flash 512KB;
- Bộ nhớ SRAM 96KB;
- Điện áp hoạt động 1.71 - 3.6VDC;
- Có 51 chân GPIO;
- Hỗ trợ 5 kênh giao tiếp UART;
- Hỗ trợ 4 kênh giao tiếp SPI với tốc độ tối đa 18Mbit/s.

Số hiệu chân	Tên chân	Chức năng được cấu hình	Mô tả tín hiệu
1	PA1	RD485	Điều khiển đầu ra 485
2	PA2	TX2	Đầu ra bản tin thời gian cho mạch connector
3	PA3	RX2	
4	PA5	SCLK	Giao tiếp SPI với W5500
5	PA6	MISO	
6	PA7	MOSI	
7	PB0	SCSn	
8	PB1	INTn	
9	PB2	RSTn	
10	PC8	I2C3 SDA	Giao tiếp I2C với khối thời gian thực
11	PC9	I2C3 SCL	
12	PC6	LCD_D4	Giao tiếp với LCD
13	PC7	LCD_D5	
14	PC8	LCD_D6	
15	PC9	LCD_D7	
16	PB12	LCD_D0	
17	PB13	LCD_D1	
18	PB14	LCD_D2	
19	PB15	LCD_D3	
20	PB0	LCD_RW	
21	PB1	LCD_RS	
22	PB2	LCD_EN	
23	PC10	GPS2	Điều khiển mạch LED
24	PC11	GPS1	
25	PC12	CPU	
26			
27			
28			

Giải pháp thiết kế giám sát nguồn: đo điện áp các nguồn 12V, 5V, 3.3V bằng phương pháp chia trở và đưa tín hiệu giám sát về các chân ADC của vi xử lý. Các trạng thái điện áp nguồn được giám sát và truy cập qua SNMP.
Giải pháp thiết kế khối LCD và Led: bao gồm các cổng cắm giắc, dây cáp tới các mạch LED, mạch GPS, mạch giao tiếp, LCD. LCD lựa chọn loại 40x2 để hiển thị nhiều thông tin hơn.
 
Hình 7: Sơ đồ nguyên lý mạch LED

Các Led trạng thái gồm:
- Led GPS1: bộ thu GPS1 hoạt động bình thường
- Led GPS2: bộ thu GPS2 hoạt động bình thường
- Led PWR: nguồn đồng hồ master hoạt động bình thường
- Led GPS: đồng hồ master hoạt động bình thường
Giải pháp thiết kế khối thời gian thực:
Chức năng, yêu cầu:
Lưu trữ và giữ xung nhịp thời gian chuẩn để cung cấp thời gian cho vi xử lý khi cần thiết. Thời gian được lưu trữ trên khối thời gian thực kể cả khi mất điện. 
-	Đầu vào: tín hiệu dao động từ thạch anh.
-	Đầu ra: tín hiệu thời gian gửi ra cổng I2C
Giải pháp thiết kế: Sử dụng IC DS3231 của hãng Maxim để giao tiếp với vi xử lý qua I2C để lưu trữ thời gian thực. Đây là dòng IC thời gian thực chính xác nhất (Extremely Accurate) của hãng MAXIM- hãng có truyền thống sản xuất IC thời gian thực hàng đầu thế giới. Ngoài ra DS3231 có cung cấp các thanh ghi để hiệu chỉnh độ chính xác về thời gian, vi xử lý có thể cấu hình lại thanh ghi này để có độ chính xác tốt hơn (Calibration).
 Các thông số cơ bản của IC DS3231 như sau:
- Độ chính xác 2ppm (sai số tối đa khi chưa hiệu chỉnh bộ giao động);
- Hỗ trợ giao tiếp I2C với tốc độ 400KHz
- Điện áp hoạt động 3 - 5.5VDC;
 
Hình 8: Sơ đồ nguyên lý khối thời gian thực
 
Giải pháp thiết kế khối nguồn mạch main: bao gồm các khối tạo nguồn ra các điện áp một chiều 12V, 5V và 3.3V cho các mạch trong đồng hồ master.
Đầu vào: điện áp 1 chiều từ 24V tới 48V DC
Đầu ra: điện áp 12V, 5V và 3.3V cho các mạch trong đồng hồ master
- LCD và các IC chức năng RS232, RS485 sử dụng điện áp 5V
- Khối IRIG-B cần điện áp 12V để tạo ra xung IRIG-B 10Vpp
- Vi xử lý sử dụng điện áp 3.3V
Tính toán năng lượng tiêu thụ cho đồng hồ master
STT	Tên linh kiện	Số lượng	Điện áp
(V)	Dòng điện
tiêu thụ
(mA)	Công suất
tiêu thụ
(mW)
1	Mạch main	1	3.3	100	330
2	Mạch GPS	1	3.3	100	330
3	Mạch LED	1	3.3	50	165
4	LCD	1	5	100	500
5	 Mạch giao tiếp	1	12	500	6000
Tổng năng lượng cần thiết tối đa (mW)	7325

Qua bảng tính toán năng lượng tiêu thu tối đa ta thấy:
- Công suất đủ của bộ nguồn 12V là 6W
- Công suất đủ của bộ nguồn 5V là 0.5W
- Công suất đủ của bộ nguồn 3.3V là 0.795W
- Công suất nguồn từ đầu vào là 7.325W
Lựa chọn nguồn DC-DC 12V là PRD15-Q48-S12-N-D với đầu vào 12-72V và đầu ra 12V 1.3A 15W
Lựa chọn nguồn DC-DC 5V là P78E05-1000 với đầu vào 8-36V và đầu ra 5V 1A 5W
Lựa chọn nguồn DC-DC 3.3V là P78E03-1000 với đầu vào 8-36V và đầu ra 3.3V 1A 3.3W

 
Hình 9: Sơ đồ nguyên lý nguồn 12V
 
Hình 10: Sơ đồ nguyên lý nguồn 5V

 
Hình 11: Sơ đồ nguyên lý nguồn 3.3V
Để tăng chủ động chế tạo, nâng cao tính ổn định khắc phục khó khăn về nguồn cung linh kiện sẽ thử nghiệm một số module nguồn của khác như nguồn DC-DC 12V, P/N: SD-15B-12, Module nguồn DC-DC 5V chân cắm, K7805M*, Module nguồn DC-DC 3.3V chân cắm, PN:K783V3* .

	Giải pháp thiết kế phần mềm nhúng mạch main
Phần mềm nhúng được lập trình C/C++ trên phần mềm Keil C cho vi xử lý ARM (đang được sử dụng tại phòng thí nghiệm điện tử). Giao diện hiển thị, cài đặt của thiết bị được lập trình webserver bằng ngôn ngữ HTML nhúng vào phần mềm nhúng của vi xử lý trên mạch.
Chức năng, yêu cầu:
- Nhận, phân tích, bóc tách bản tin GPS: Các bản tin GPS sẽ là các bản tin UART chứa thông tin về thời gian. Nhận các thông tin về các IP của mạch giao tiếp.
- Hiển thị thông tin thời gian, thông tin trạng thái các tín hiệu GPS trên màn hình LCD. Trong trường hợp không có tín hiệu GPS sẽ sử dụng thời gian từ chip thời gian thực (realtime clock) trên mạch main. 
- Gửi thông tin thời gian đến mạch giao tiếp qua giao thức UART. 
- Giám sát thông tin trạng thái tín hiệu GPS, trạng thái nguồn của đồng hồ master và gửi thông tin đến mạch giao tiếp qua giao thức UART. 
Sơ đồ khối phần mềm nhúng chip vi xử lý trung tâm mạch main
 
Hình 12: Sơ đồ khối phần mềm nhúng

- Vi điều khiển sử dụng là STM32 sẽ được lập trình bằng phần mềm Keil C cho ARM (đang được sử dụng tại phòng thí nghiệm điện tử).
Giải pháp lấy thông tin thời gian từ bản tin GPS: thông tin thời gian được phân tích, bóc tách từ bản tin GPS GPZDA có cấu trúc như sau: 
 
Hình 13: Cấu trúc bản tin GPZDA

Lưu đồ thuật toán phần mềm nhúng mạch main đồng hồ GPS master:
 
Hình 14: Lưu đồ thuật toán phần mềm nhúng mạch main

c)	Giải pháp thiết kế các bộ chuyển đổi nguồn đầu vào đồng hồ master:
Tùy theo phương án cấp nguồn mà khách hàng chọn thì có thể lắp tối đa 02 bộ chuyển đổi nguồn AC/DC. Các lựa chọn như sau:
-	01 đầu vào AC 220V và 01 đầu vào DC 24:48V.
-	02 đầu vào AC 220V.
Đầu vào: điện áp 220V hoặc DC 24:48V
Đầu ra: điện áp một chiều từ 24V vào mạch main
 
Hình 15: Giải pháp thiết kế nguồn đồng hồ master

Lựa chọn bộ chuyển đổi nguồn AC-DC 24V 1.3A làm bộ chuyển đổi nguồn AC-DC cho đồng hồ master. Ngoài ra thử nghiệm thêm nguồn AC-DC 12V, P/N: IRM-30-12ST thay thế nếu bộ nguồn DC-DC 24V sang 12V hết hàng.

d)	Giải pháp thiết kế mạch giao tiếp
Chức năng: 
-	Nhận dữ liệu thời gian, nguồn, và các thông tin khác từ mạch main của đồng hồ GPS master
-	Chuyển đổi dữ liệu nhận được thành các dạng dữ liệu RS232, RS485, IRIG-B, RS232 NTP server để cung cấp cho đồng hồ slave và các thiết bị khác như ghi âm, máy tính, …
-	Tạo dữ diệu giám sát SNMP.
- Tạo tín hiệu dạng IRIG-B xoay chiều (AC)sử dụng biến đổi DAC biên độ từ - 5V đến 5V.
Đầu vào:
-	 Thông tin thời gian chuẩn, trạng thái các bộ thu GPS, trạng thái nguồn qua UART TTL từ mạch main.
Đầu ra:
-	   06 đầu ra RS485 cung cấp thông tin thời gian.
-	   02 đầu ra IRIGB 10Vpp cung cấp thông tin thời gian.
-	   01 đầu ra RS232 cung cấp thông tin thời gian.
-	03 cổng RJ45 NTP time server cung cấp thông tin thời gian, dữ liệu SNMP cung cấp thông tin giám sát.

Giải pháp thiết kế:
 
Hình 16: Sơ đồ khối mạch giao tiếp

Các khối chức năng của mạch giao tiếp:
•	 Khối vi xử lý
•	 Khối NTP-SNMP
•	 Khối IRIG-B
•	 Khối RS485
•	 Khối RS232
Thiết kế chi tiết:
•	 Khối xử lý mạch giao tiếp: sử dụng vi xử lý STM32 làm vi xử lý chính. Thời gian thực chuyển sang các dạng RS232, RS484 và IRIG-B. Tín hiệu IRIG-B được tạo bảo chức năng DAC của vi xử lý stm32. Các thông tin hoạt động được đọc và ghi vào IC nhớ AT24C16D-SSHM-T qua giao tiếp I2C.
Chức năng, yêu cầu:
-	Xử lý tách thông tin mạch main từ ngõ vào RX của UART;
-	Tạo tín hiệu IrigB từ DAC với biên độ 0-3.3V
-	Tạo thời gian chuẩn qua NTP và các thông số giám sát qua SNMP
-	Chuyển đổi thời gian chuẩn thành các bản tin thời gian qua cổng UART TTL cho khối RS485, RS232.
-	Xử lý giao tiếp với W5500 qua giao tiếp SPI để xử lý các gói tin TCP, UDP đồng thời nhận thời gian từ mạch main qua cổng UART
-	Đọc và ghi thông tin hoạt động như IP của các cổng RJ45 từ IC nhớ, gửi thông tin IP này cho mạch main.
Đầu vào: Thông tin thời gian chuẩn, trạng thái các bộ thu GPS, trạng thái nguồn qua UART TTL từ mạch main.
Đầu ra: 
-	Bản tin thời gian UART TTL tới các khối RS485, RS232
Số hiệu chân	Tên chân	Chức năng được cấu hình	Mô tả tín hiệu
1	PA1	RD485	Điều khiển đầu ra 485
2	PA2	TX2	Đầu ra bản tin thời gian cho mạch connector
3	PA3	RX2	
4	PA5	SCLK	Giao tiếp SPI với W5500
5	PA6	MISO	
6	PA7	MOSI	
7	PB0	SCSn	
8	PB1	INTn	
9	PB2	RSTn	
10	PC8	I2C3 SDA	Giao tiếp I2C với IC nhớ
11	PC9	I2C3 SCL	

•	Chức năng, yêu cầu khối NTP-SNMP: 
-	Chức năng: 
+ Có 03 cổng RJ45 độc lập cung cấp 3 cổng NTP độc lập. 
+ Tạo các bản tin NTP và SNMP. Đưa lên đường truyền ethernet thông qua cổng RJ45 dưới dạng các bản tin TCP hoặc UDP. 
-	Đầu vào: Dữ liệu thời gian từ chip vi xử lý mạch giao tiếp thông qua giao tiếp SPI. 
-	Đầu ra: Dữ liệu NTP, SNMP trên đường truyền ethernet sử dụng cổng RJ45. 
Giải pháp thiết kế khối NTP-SNMP:
- Khối giao tiếp mạng sử dụng IC chuyên dụng W5500 của hãng Wiznet. Các bản tin qua mạng internet sẽ được W5500 tự động xử lý và chuyển về các bản tin TCP, UDP. Qua giao tiếp SPI với vi xử lý, vi xử lý sẽ nhận được các bản tin này và trên cơ sở đó xử lý các dữ liệu đến và đi. Các bản tin SNMP tới sẽ chuyển thành các bản tin UDP và vi xử lý sẽ kiểm tra các thông tin trong bảng dữ liệu SMNP và phản hồi các bản tin UDP qua W5500 ra mạng internet. 
Để đảm bảo hiệu năng hoạt động của giao thức NTP, các linh kiện của khối giao tiếp NTP bao gồm vi xử lý stm32, IC W5500, cổng mạng được  tăng thêm 2 khối, tổng cộng sẽ có 3 khối linh kiện có đảm bảo có 03 cổng ngoại vi RJ45 cung cấp chức năng NTP time server. Dữ liệu thời gian từ mạch main sẽ gửi tới cả 3 vi xử lý, các vi xử lý sẽ xử lý thời gian đảm bảo tính năng NTP time server. Thông tin về cấu hình của thiết bị sẽ được lưu ở trong IC nhớ, tránh việc Flash của STM32 phải ghi dữ liệu nhiều lần. Khi hoạt động STM32 sẽ đọc thông tin trong IC nhớ để đọc và ghi cấu hình hoạt động. Một STM32 sẽ gửi và nhận thông tin với mạch in để gửi IP cho mạch main.   

 
Hình 17: Sơ đồ nguyên lý giao tiếp mạng sử dụng IC W5500.

•	 Khối IRIG-B
Chức năng, yêu cầu:
-	Chức năng:  Nhận bản tin chứa các tham số thời gian từ mạch main dưới dạng tín hiệu UART chuyển thành các tín hiệu IRIG-B 10Vpp cách ly
-	Đầu vào: bản tin UART TTL chứa thông tin về thời gian
-	Đầu ra: tín hiệu IRIG-B chuẩn sin
Giải pháp thiết kế:
Đối với tín hiệu IRIG-B, lựa chọn IC khuếch đại của hãng JRC là NJM2113D. Đây là IC khuếch đại tín hiệu chuyên dụng, đơn giản dễ sử dụng. Đầu ra của IC khuếch đại là biến áp cách ly. Biến áp có nhiệm vụ khuếch đại biên độ và cách ly tín hiệu. Thử nghiệm biến áp cách ly Triad TY-145P để thêm lựa chọn biến áp cách ly.
 
Hình 18: Sơ đồ nguyên lý khối khuếch đại và cách ly tín hiệu IRIG-B

•	 Khối RS485
Chức năng, yêu cầu: Chuyển bản tin thời gian về dạng vật lý RS485 để truyền tới các đồng hồ slave 
-	Đầu vào: bản tin thời gian uart ttl từ khối vi xử lý
-	Đầu ra: bản tin thời gian RS485
Giải pháp thiết kế:
Lựa chọn IC dán ADM2587EBRWZ có chức năng truyền thông RS485 cách ly. Dữ liệu RS485 được chuyển hóa thành dữ liệu UART TTL. IC ADM2587EBRWZ xử lý với tốc độ dữ liệu là 250kbps, hỗ trợ 01 cổng truyền thông. IC tương thích tốt với cả điện áp 3-5.5V. 
 
Hình 19: Sơ đồ nguyên lý truyền thông RS485

•	 Khối RS232
Chức năng, yêu cầu: truyền thông tin về thời gian qua cổng RS232
-	Đầu vào: bản tin thời gian dưới dạng UART TTL
-	Đầu ra: bản tin thời gian chuẩn RS232
Giải pháp thiết kế:
Lựa chọn IC chuyên dụng MAX3232CDR có chức năng chuyển đổi tín hiệu truyền thông RS232 (mức logic -12V 12V) thành tín hiệu UART TTL(0-5VDC) để đưa tới cổng giao tiếp của chip vi xử lý. IC MAX3232CDR xử lý với tốc độ dữ liệu là 250kbps, hỗ trợ 01 cổng truyền thông. IC tương thích tốt với cả điện áp 3-5.5V. Cổng giao tiếp vật lý tại thiết bị được thiết kế sử dụng loại cổng DB9 cái.
 
Hình 20: Sơ đồ nguyên lý khối giải mã tín hiệu RS232

Giải pháp thiết kế phần mềm nhúng mạch giao tiếp
Chức năng, yêu cầu:
Chức năng NTP time server: Nhận bản tin thông tin thời gian (gồm ngày, tháng, năm, giờ, phút, giây) từ module GPS master, phân tích bản tin để lấy các thông tin cụ thể làm cơ sở tạo bản tin NTP. Khi có thiết bị gửi yêu cầu trên NTP, vi điều khiển sẽ tính toán và trả về bản tin chứa thông tin thời gian qua giao thức NTP.
Chức năng SNMP: cập nhật các trạng thái hoạt động của đồng hồ master như trạng thái các chip thu GPS1, GPS2, nguồn 1, nguồn 2 vào bảng trạng thái SNMP để cung cấp cho các máy tính quản lý qua SNMP.
Giải pháp thiết kế:
Vi điều khiển sử dụng là STM32G474RBT6 sẽ được lập trình bằng phần mềm Keil C cho ARM (đang được sử dụng tại phòng thí nghiệm điện tử).
 
Hình 21: Sơ đồ khối phần mềm nhúng mạch giao tiếp

Thư viện lập trình IC W5500 được cung cấp bởi nhà sản xuất Wiznet. Phần mềm nhúng sẽ thực hiện kết cấu hình nối phần cứng giữa STM32 và W5500. Sau đó dựa trên thư viện tầng truyền thông UDP và TCP của nhà sản xuất Wiznet để thực hiện chức năng SNMP
Phần mềm nhúng mạch giao tiếp cho từng MCU:
-	01 MCU chính ngoài các tính năng như trên còn nhận thông tin từ 2 MCU còn lại để gửi về mạch main thông tin IP của 02 cổng NTP còn lại.
-	02 MCU còn lại chỉ cần đảm bảo tính năng NTP time server, các tính năng IRIGB và gửi thông tin qua cổng RS485/RS232 sẽ được tắt đi. Ngoài ra các MCU này phải gửi thông tin IP hoạt động cho MCU chính.
Cài đặt cấu hình cho thiết bị: các cổng NTP cần cấu hình các tham số phụ thuộc vào mạng cục bộ mà thiết bị triển khai như IP của thiết bị. Để thực hiện chức năng này một webserver sẽ được nhúng trong chương trình của STM32. Người khai thác sẽ sử dụng một thiết bị khác (máy tính) truy cập vào địa chỉ IP của cổng NTP/SNMP. Một trang web cấu hình sẽ hiện ra cho phép cài đặt các tham số cần cấu hình như IP cổng 1, 2 và 3.  
 
 
Hình 22: Lưu đồ thuật toán phần mềm nhúng mạch giao tiếp














4.2.2 Giải pháp thiết kế đồng hồ Slave treo tường dùng LED 7 thanh
a) Sơ đồ khối đồng hồ Slave 
 
Hình 23: Sơ đồ khối đồng hồ slave
Mô tả hoạt động của thiết bị đồng hồ slave:
Đồng hồ slave dùng để hiển thị thời gian nên được cung cấp khối hiển thị rõ ràng, dễ quan sát thời gian. Tín hiệu thời gian chuẩn được cung cấp qua RS485 hoặc qua NTP. Khối vi xử lý sẽ bóc tách bản tin thời gian và hiển thị qua LED 7 thanh hoặc LED ma trận. Thời gian được lưu trữ qua các IC thời gian thực để có thể lưu được thời gian khi mất đồng bộ hoặc mất điện.
Để thuận tiện khai thác, thiết kế các mạch của đồng hồ slave treo tường dùng LED 7 thanh được phân chia như sau:
- Mạch điều khiển slave: bao gồm khối vi xử lý, khối đồng hồ thời gian thực, khối nguồn vi xử lý, khối NTP client
- Mạch hiển thị LED: hiển thị các thông tin thời gian 
b) Giải pháp thiết kế mạch điều khiển slave
Chức năng: Mạch điều khiển slave nhận thời gian từ đồng hồ master, lưu trữ thông tin về thời gian vào IC thời gian thực và hiển thị thông tin về thời gian trên các mạch hiển thị LED. Mạch được thiết kế để có thể sử dụng chung cho tất cả các loại đồng hồ slave.
-	Đầu vào: tín hiệu RS485 hoặc NTP từ đồng hồ master
-	Đầu ra: tín hiệu điều khiển các LED hiển thị
Mạch điều khiển slave có các khối:
-	Khối vi xử lý STM32
-	Khối thời gian thực
-	Khối NTP/RS485
Các khối này có thiết kế tượng tự với các khối tương ứng của đồng hồ master.
c) Giải pháp thiết kế mạch hiển thị LED
Chức năng, yêu cầu:
Chuyển đổi thời gian dạng thập phân thành các tín hiệu điều khiển LED ma trận hoặc LED 7 thanh
Giải pháp thiết kế:
Để thực hiện chức năng này chủ nhiệm nhiệm vụ lựa chọn sử dụng IC điều khiển LED 7 thanh và Led ma trận chuyên dụng MAX7221/7219
 
Hình 24: Sơ đồ nguyên lý MAX7221/7219

Các IC MAX7221/7219 có thể mắc nối tiếp với nhau để điều khiển nhiều cụm LED 7 thanh hoặc LED ma trận. Dữ liệu cần hiển thị sẽ đưa đến bộ nhớ hiển thị của IC MAX7221/7219, và IC MAX7221/7219 tự động quét hiển thị. IC MAX7221/7219 sử dụng truyền thông nối tiếp có đồng bộ.
Mạch có cáp nối tới mạch main slave và đấu nối ra bên ngoài thiết bị.
- Đầu vào: tín hiệu điều khiển và dữ liệu để điều khiển các IC MAX7221/7219
- Đầu ra: hiển thị thời gian trên LED
d) Giải pháp thiết kết phần mềm nhúng đồng hồ slave
Do các đồng hồ slave có cấu trúc tương tự nhau và các khối chức năng tương ứng tương tự đồng hồ master nên cấu trúc phần mềm nhúng tương tự với đồng hồ master.
 
Hình 25: Sơ đồ khối phần mềm nhúng đồng hồ slave
 
Hình 26: Lưu đồ thuật toán mềm nhúng đồng hồ slave

4.2.3 Giải pháp thiết kế đồng hồ Slave để bàn console dùng LED 7 thanh
Cấu trúc và giải pháp tương tự đồng hồ Slave treo tường dùng LED 7 thanh bao gồm 2 mạch:
- Mạch điều khiển: sử dụng chung mạch điều khiển với đồng hồ Slave treo tường dùng LED 7
- Mạch hiển thị LED: sử dụng chung IC điều khiển LED MAX7221/7219
Giải pháp thiết kế phần mềm nhúng: tương tự đồng hồ slave treo tường.
4.2.4 Giải pháp thiết kế đồng hồ Slave treo tường dùng LED ma trận
Cấu trúc và giải pháp tương tự đồng hồ Slave treo tường dùng LED 7 thanh
- Mạch điều khiển: sử dụng chung mạch điều khiển với đồng hồ Slave treo tường dùng LED 7
- Mạch hiển thị LED: sử dụng chung IC điều khiển LED MAX7221/7219
4.2.5.	Giải pháp thiết kế vỏ thiết bị
 Vỏ thiết bị sử dụng vật liệu nhôm, được chế tạo bằng công nghệ CNC kết hợp gia công cơ khí và anode hóa.


Hình 27: Minh họa vỏ thiết bị
5.	Giải pháp công nghệ chế tạo
Thực hiện chế tạo sản phẩm theo phương án: 
-	 Mua các thiết bị, vật tư phần cứng.
-	 Lắp ráp phần điều khiển
-	 Sản xuất các bộ phận cơ khí
-	 Lắp ráp hoàn thiện thiết bị
-	 Nạp phần mềm nhúng.
-	 Chạy thử và hiệu chỉnh thông số kỹ thuật.
-	 Hoàn thiện sản phẩm.
 

6.	Quy trình và thiết bị công nghệ sử dụng chế tạo sản phẩm
STT	Thiết bị/ Dụng cụ	Đơn vị
tính	Số lượng	Ghi chú
1	Máy hiện sóng	Chiếc	01	
Sử dụng nguồn lực sẵn có của công ty TNHH kỹ thuật quản lý bay.
2	Mỏ hàn điện đa năng	Chiếc	02	
3	Dụng cụ sửa chữa điện tử	Bộ	01	
4	Đồng hồ đo vạn năng 	Cái	02	
5	Ampe kìm	Cái	01	
6	Kìm bấm dây	Cái	01	
7	Kìm cắt	Cái	01	

7.	Tiêu chí lựa chọn nhà cung ứng sản phẩm dịch vụ để mua linh kiện, thiết bị
-	Đảm bảo tính pháp lý
-	Đảm bảo chất lượng sản phẩm
-	Có khả năng cung ứng lâu dài, ổn định
-	Có kinh nghiệm trong lĩnh vực điện, điện tử, tự động hóa
-	Có dịch vụ sau bán hàng tốt
-	Nhà cung ứng có áp dụng hệ thống quản lý chất lượng ISO 9001:2015 là một yếu tố ưu tiên. 
