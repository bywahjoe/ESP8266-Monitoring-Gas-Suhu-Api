<?php

$servername = "localhost";
$database   = "bywahjoe_asap";
$userID     = "bywahjoe_asap";
$pass       = "Wahyu123-";

$myAPI="sendIP4";
$apiKEY= $d1= $d2= $d3= $d4="";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $apiKEY = test_input($_POST["apiKEY"]);
    if($apiKEY == $myAPI) {
        $d1 = test_input($_POST["d1"]);
        $d2 = test_input($_POST["d2"]);
        $d3 = test_input($_POST["d3"]);
        $d4 = test_input($_POST["d4"]);
        
        // Create connection
        $conn = new mysqli($servername, $userID, $pass, $database);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
        $sql = "INSERT INTO sensor (GAS,API,SUHU,RELAY) VALUES ('" . $d1 . "','" . $d2 . "','" . $d3 . "','" . $d4 . "') ";
        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    
        $conn->close();
    }
    else {
        echo "Wrong API Key provided.";
    }

}
else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
?>