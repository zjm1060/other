		"/*未过期的会议室预约提醒*/\n" +
		"\n" + 
		"--预约中的会议室不存在：\n" + 
		"SELECT  1 AS id ,\n" + 
		"        '预约中的会议室不存在' AS name ,\n" + 
		"        COUNT(*) qty ,\n" + 
		"        1 AS type\n" + 
		"FROM    xmis.t_meetingroom_app_form f\n" + 
		"        LEFT JOIN t_meetingroom_info room ON f.room_id = room.room_id\n" + 
		"WHERE   f.status_id = 3\n" + 
		"        AND GETDATE() <= f.begin_datetime\n" + 
		"        AND ( room.room_No IS  NULL\n" + 
		"              OR room.room_No = ''\n" + 
		"            )\n" + 
		"UNION\n" + 
		"--新的会议室申请：\n" + 
		"SELECT  2 ,\n" + 
		"        '新的会议室申请' ,\n" + 
		"        COUNT(*) ,\n" + 
		"        1 AS type\n" + 
		"FROM    t_meetingroom_app_form\n" + 
		"WHERE   status_id = 1\n" + 
		"        AND GETDATE() <= begin_datetime\n" + 
		" --assigned_datetime IS null\n" + 
		"UNION\n" + 
		"--申请人申请变更已批准的会议室预约：\n" + 
		"SELECT  3 ,\n" + 
		"        '申请人申请变更已批准的会议室预约' ,\n" + 
		"        COUNT(*) ,\n" + 
		"        1 AS type\n" + 
		"FROM    t_meetingroom_app_form\n" + 
		"WHERE   status_id = 2\n" + 
		"        AND GETDATE() <= begin_datetime\n" + 
		"        AND app_datetime > assigned_datetime\n" + 
		"UNION\n" + 
		"--审批人变更会议室预约：\n" + 
		"SELECT  4 ,\n" + 
		"        '审批人变更会议室预约' ,\n" + 
		"        COUNT(*) ,\n" + 
		"        1 AS type\n" + 
		"FROM    t_meetingroom_app_form\n" + 
		"WHERE   status_id = 2\n" + 
		"        AND GETDATE() <= begin_datetime\n" + 
		"        AND app_datetime < assigned_datetime\n" + 
		"UNION\n" + 
		"--申请人申请撤销已批准的会议室预约：\n" + 
		"SELECT  5 ,\n" + 
		"        '申请人申请撤销已批准的会议室预约' ,\n" + 
		"        COUNT(*) ,\n" + 
		"        1 AS type\n" + 
		"FROM    t_meetingroom_app_form\n" + 
		"WHERE   status_id = 4\n" + 
		"        AND GETDATE() <= begin_datetime\n" + 
		"        AND app_datetime > assigned_datetime\n" + 
		"UNION\n" + 
		"--审批人撤销会议室预约：\n" + 
		"SELECT  6 ,\n" + 
		"        '审批人撤销会议室预约' ,\n" + 
		"        COUNT(*) ,\n" + 
		"        1 AS type\n" + 
		"FROM    t_meetingroom_app_form\n" + 
		"WHERE   status_id = 4\n" + 
		"        AND GETDATE() <= begin_datetime\n" + 
		"        AND app_datetime < assigned_datetime\n" + 
		"        /*未过期的公务车预约提醒*/\n" + 
		"\n" + 
		"--预约中的公务车不存在：\n" + 
		"\n" + 
		"union\n" + 
		"SELECT  11 AS id ,\n" + 
		"        '预约中的公务车不存在' AS name ,\n" + 
		"        COUNT(*) qty ,\n" + 
		"        2 AS type\n" + 
		"FROM    t_car_app_carid_detail d\n" + 
		"        RIGHT OUTER JOIN t_car_app_form f ON f.form_id = d.form_id\n" + 
		"        LEFT OUTER JOIN t_car_info c ON d.car_id = c.car_id\n" + 
		"WHERE   f.status_id = 3\n" + 
		"        AND ( c.car_No IS NULL\n" + 
		"              OR c.car_No = ''\n" + 
		"            )\n" + 
		"UNION\n" + 
		"--新的公务车申请：\n" + 
		"SELECT  12 ,\n" + 
		"        '新的公务车申请' ,\n" + 
		"        COUNT(*) ,\n" + 
		"        2 AS type\n" + 
		"FROM    t_car_app_form\n" + 
		"WHERE   status_id = 1\n" + 
		"        AND GETDATE() <= begin_datetime\n" + 
		" --assigned_datetime IS null\n" + 
		"UNION\n" + 
		"--申请人申请变更已批准的公务车预约：\n" + 
		"SELECT  13 ,\n" + 
		"        '申请人申请变更已批准的公务车预约' ,\n" + 
		"        COUNT(*) ,\n" + 
		"        2 AS type\n" + 
		"FROM    t_car_app_form\n" + 
		"WHERE   status_id = 2\n" + 
		"        AND GETDATE() <= begin_datetime\n" + 
		"        AND app_datetime > assigned_datetime\n" + 
		"UNION\n" + 
		"--审批人变更公务车预约：\n" + 
		"SELECT  14 ,\n" + 
		"        '审批人变更公务车预约' ,\n" + 
		"        COUNT(*) ,\n" + 
		"        2 AS type\n" + 
		"FROM    t_car_app_form\n" + 
		"WHERE   status_id = 2\n" + 
		"        AND GETDATE() <= begin_datetime\n" + 
		"        AND app_datetime < assigned_datetime\n" + 
		"UNION\n" + 
		"--申请人申请撤销已批准的公务车预约：\n" + 
		"SELECT  15 ,\n" + 
		"        '申请人申请撤销已批准的公务车预约' ,\n" + 
		"        COUNT(*) ,\n" + 
		"        2 AS type\n" + 
		"FROM    t_car_app_form\n" + 
		"WHERE   status_id = 4\n" + 
		"        AND GETDATE() <= begin_datetime\n" + 
		"        AND app_datetime > assigned_datetime\n" + 
		"UNION\n" + 
		"--审批人撤销公务车预约：\n" + 
		"SELECT  16 ,\n" + 
		"        '审批人撤销公务车预约' ,\n" + 
		"        COUNT(*) ,\n" + 
		"        2 AS type\n" + 
		"FROM    t_car_app_form\n" + 
		"WHERE   status_id = 4\n" + 
		"        AND GETDATE() <= begin_datetime\n" + 
		"        AND app_datetime < assigned_datetime\n" + 
		"ORDER BY id\n" + 
		"\n" + 
		"";