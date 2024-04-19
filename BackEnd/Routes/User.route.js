const express = require('express');
const router = express.Router();

const UserController = require('../Controllers/User.Controller');

//Get a list of all Users
router.get('/', UserController.getAllUsers);

//Create a new User
router.post('/', UserController.createNewUser);

//Get a user by id
router.get('/:id', UserController.findUserById);

// Delete a User by id
router.delete('/:id', UserController.deleteAUser);

module.exports = router;
