const mongoose = require('mongoose');
const Schema = mongoose.Schema;



const UserSchema = new Schema({
  full_name: {
    type: String
  },
  username: {
    type: String
  },
});

const User = mongoose.model('User', UserSchema);
module.exports = User;
