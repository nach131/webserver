<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Procesando datos POST</title>
</head>
<body>
    <h2>Datos recibidos:</h2>
    <?php
    // Verifica si se han recibido datos POST
    if ($_SERVER["REQUEST_METHOD"] == "POST") {
        // Verifica si se han recibido los campos "name" y "age"
        if (isset($_POST["name"]) && isset($_POST["age"])) {
            // Obtiene los valores de los campos "name" y "age"
            $name = $_POST["name"];
            $age = $_POST["age"];

            // Muestra los datos recibidos
            echo "<p>Nombre: $name</p>";
            echo "<p>Edad: $age</p>";
        } else {
            // Muestra un mensaje de error si faltan campos
            echo "<p>Error: Faltan datos POST</p>";
        }
    } else {
        // Muestra un mensaje si la solicitud no es POST
        echo "<p>Error: Esta página solo acepta solicitudes POST</p>";
    }
    ?>
</body>
</html>
