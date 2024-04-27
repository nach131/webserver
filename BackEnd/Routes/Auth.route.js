const express = require('express');
const router = express.Router();

const UserController = require('../Controllers/Auth.Controlle');

router.post('/register', createNewUser);

router.post('/login', loginUser);
