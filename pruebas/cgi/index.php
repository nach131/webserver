<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Formulario POST</title>
</head>
<body>
    <h2>Formulario POST</h2>
    <form action="process.php" method="POST">
        <label for="name">Nombre:</label><br>
        <input type="text" id="name" name="name"><br>
        <label for="age">Edad:</label><br>
        <input type="text" id="age" name="age"><br><br>
        <input type="submit" value="Enviar">
    </form>
</body>
</html>
