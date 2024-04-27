const createError = require('http-errors');
const mongoose = require('mongoose');

const User = require('../Models/User.model');

module.exports = {

	newUser: async (req, res, next) => {
		try {
			//   const results = await User.find({}, { __v: 0 });
			// const results = await User.find({}, { name: 1, price: 1, _id: 0 });
			// const results = await User.find({ price: 699 }, {});
			//   res.send(results);
			res.send("registro")
		} catch (error) {
			console.log(error.message);
		}
	},

	loginUser: async (req, res, next) => {
		try {
			//   const results = await User.find({}, { __v: 0 });
			// const results = await User.find({}, { name: 1, price: 1, _id: 0 });
			// const results = await User.find({ price: 699 }, {});
			//   res.send(results);
			res.send("login")
		} catch (error) {
			console.log(error.message);
		}
	},
}