/*
delete xmis.add_城市
delete xmis.add_地址
delete xmis.add_省
delete xmis.sys_婚姻状态
delete xmis.sys_货币
delete xmis.sys_午餐地址表
delete xmis.sys_性别
delete xmis.sys_政治面貌
delete xmis.con_合同来源类型
delete xmis.con_合同类型
*/


bcp "xmis.add_城市" out "xmis.add_城市.txt" -c -q -S "192.169.11.140" -U "xmis-uat" -P "xmis"                  
bcp "xmis.add_地址" out "xmis.add_地址.txt" -c -q -S "192.169.11.140" -U "xmis-uat" -P "xmis"                  
bcp "xmis.add_省" out "xmis.add_省.txt" -c -q -S "192.169.11.140" -U "xmis-uat" -P "xmis"                  
bcp "xmis.sys_婚姻状态" out "xmis.sys_婚姻状态.txt" -c -q -S "192.169.11.140" -U "xmis-uat" -P "xmis"                  
bcp "xmis.sys_货币" out "xmis.sys_货币.txt" -c -q -S "192.169.11.140" -U "xmis-uat" -P "xmis"                  
bcp "xmis.sys_午餐地址表" out "xmis.sys_午餐地址表.txt" -c -q -S "192.169.11.140" -U "xmis-uat" -P "xmis"                  
bcp "xmis.sys_性别" out "xmis.sys_性别.txt" -c -q -S "192.169.11.140" -U "xmis-uat" -P "xmis"                  
bcp "xmis.sys_政治面貌" out "xmis.sys_政治面貌.txt" -c -q -S "192.169.11.140" -U "xmis-uat" -P "xmis"                  
bcp "xmis.con_合同来源类型" out "xmis.con_合同来源类型.txt" -c -q -S "192.169.11.140" -U "xmis-uat" -P "xmis"                  
bcp "xmis.con_合同类型" out "xmis.con_合同类型.txt" -c -q -S "192.169.11.140" -U "xmis-uat" -P "xmis"                  

bcp "xmis.add_城市" in "xmis.add_城市.txt" -c -q -S "192.169.11.6\JKGL" -U "xmis" -P "xmis"                  
bcp "xmis.add_地址" in "xmis.add_地址.txt" -c -q -S "192.169.11.6\JKGL" -U "xmis" -P "xmis"                  
bcp "xmis.add_省" in "xmis.add_省.txt" -c -q -S "192.169.11.6\JKGL" -U "xmis" -P "xmis"                  
bcp "xmis.sys_婚姻状态" in "xmis.sys_婚姻状态.txt" -c -q -S "192.169.11.6\JKGL" -U "xmis" -P "xmis"                  
bcp "xmis.sys_货币" in "xmis.sys_货币.txt" -c -q -S "192.169.11.6\JKGL" -U "xmis" -P "xmis"                  
bcp "xmis.sys_午餐地址表" in "xmis.sys_午餐地址表.txt" -c -q -S "192.169.11.6\JKGL" -U "xmis" -P "xmis"                 
bcp "xmis.sys_性别" in "xmis.sys_性别.txt" -c -q -S "192.169.11.6\JKGL" -U "xmis" -P "xmis"                  
bcp "xmis.sys_政治面貌" in "xmis.sys_政治面貌.txt" -c -q -S "192.169.11.6\JKGL" -U "xmis" -P "xmis"  
bcp "xmis.con_合同来源类型" in "xmis.con_合同来源类型.txt" -c -q -S "192.169.11.6\JKGL" -U "xmis" -P "xmis"                  
bcp "xmis.con_合同类型" in "xmis.con_合同类型.txt" -c -q -S "192.169.11.6\JKGL" -U "xmis" -P "xmis"                  
