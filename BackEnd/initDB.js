const mongoose = require('mongoose');
const dotenv = require('dotenv').config();

let URL;

if (process.env.NODE_ENV === 'production') {
  // Configuraciones específicas de producción
  URL = process.env.MONGODB_URI;
} else {
  // Configuraciones específicas de desarrollo
  URL = process.env.MONGODB_DEV;
}

console.log(URL)

module.exports = () => {
  mongoose
    .connect(URL, {
      dbName: process.env.DB_NAME,
      user: process.env.DB_USER,
      pass: process.env.DB_PASS,
      useNewUrlParser: true,
      useUnifiedTopology: true,
      useFindAndModify: false
    })
    .then(() => {
      console.log('Mongodb connected....');
    })
    .catch(err => console.log(err.message));

  mongoose.connection.on('connected', () => {
    console.log('Mongoose connected to db...');
  });

  mongoose.connection.on('error', err => {
    console.log(err.message);
  });

  mongoose.connection.on('disconnected', () => {
    console.log('Mongoose connection is disconnected...');
  });

  process.on('SIGINT', () => {
    mongoose.connection.close(() => {
      console.log(
        'Mongoose connection is disconnected due to app termination...'
      );
      process.exit(0);
    });
  });
};
