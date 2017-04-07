
create table xmis.con_发票金额拆分(
	ID           int not null,
	合同ID       int not null,
	发票号       int not null,
	拆分金额     decimal(10,2),
	是否已付款   bit,
	预计付款时间 datetime,
	备注         varchar(200),
	登记时间     datetime,
	登记人ID     int
)

