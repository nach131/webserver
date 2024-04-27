db.createUser(
	{
		user: "nacho",
		pwd: "klingon",
		roles: [
			{
				role: "readWrite",
				db: "my_db"
			}
		]
	}
);
db.createCollection("test"); //MongoDB creates the database when you first store data in that database