const express = require('express');
const createError = require('http-errors');
const dotenv = require('dotenv').config();
const cors = require('cors')
const path = require('path');

const app = express();

// const corsOptions = {
//   origin: ['http://192.168.1.45', 'http://localhost:3000', 'http://localhost:5173/'],
// };
// app.use(cors(corsOptions));
app.use(cors());

app.use(express.json());
app.use(express.urlencoded({ extended: true }));

// Initialize DB
require('./initDB')();

if (process.env.NODE_ENV === 'production') {
  // app.use('/images', express.static(pescadoPath));
}

if (process.env.NODE_ENV === 'production') {
  // Configuraciones específicas de producción
  console.log('Modo de producción');
} else {
  // Configuraciones específicas de desarrollo
  console.log('Modo de desarrollo');
}

app.use(express.static('./dist'))

const UserRoute = require('./Routes/User.route');
app.use('/api/users', UserRoute);

// app.get('*', (req, res) => {
//   res.sendFile(path.join(__dirname, './dist/index.html'));
// });

//404 handler and pass to error handler
app.use((req, res, next) => {
  next(createError(404, 'Not found'));
});

//Error handler
app.use((err, req, res, next) => {
  res.status(err.status || 500);
  res.send({
    error: {
      status: err.status || 500,
      message: err.message
    }
  });
});

const PORT = process.env.PORT || 3000;

app.listen(PORT, () => {
  console.log('Server started on port ' + PORT + '...');
});
