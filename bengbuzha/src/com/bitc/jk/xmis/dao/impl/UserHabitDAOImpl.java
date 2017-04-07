package com.bitc.jk.xmis.dao.impl;

import java.util.List;

import com.bitc.jk.xmis.dao.UserHabitDAO;
import com.bitc.jk.xmis.model.sys.UserHabit;

@SuppressWarnings("unchecked")
public class UserHabitDAOImpl extends BaseDAOImpl implements UserHabitDAO {

	public UserHabit getUserHabit(int userID) {
		List<UserHabit> list = ht.findByNamedQuery("getUserHabitByUserID",
				userID);
		if (list.size() > 0) {
			return list.get(0);
		}
		return null;
	}

	public void doSaveUserHabit(UserHabit habit) {
		ht.save(habit);
	}

	public void doUpdateUserHabit(UserHabit habit) {
		ht.update(habit);
	}

}
