<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="refresh" content="5">
    <meta name="viewport" content="width=device-width, initial-scale=1">
	<title>Handle Data</title>
</head>
<body>

<?php

$servername = "localhost";
$database	= "bywahjoe_asap";
$userID		= "bywahjoe_asap";
$pass		= "Wahyu123-";

$conn = new mysqli($servername, $userID, $pass, $database);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT * FROM sensor ORDER BY ID DESC";

echo '<table cellspacing="5" cellpadding="5">
      <tr> 
        <td><b>ID</b></td> 
        <td><b>GAS</b></td> 
        <td><b>API</b></td> 
        <td><b>SUHU</b></td>
        <td><b>RELAY</b></td> 
        <td><b>SUBMIT</b></td> 
      </tr>';
 
if ($result = $conn->query($sql)) {
    while ($row = $result->fetch_assoc()) {
        $t1 = $row["ID"];
        $t2 = $row["GAS"];
        $t3 = $row["API"];
        $t4 = $row["SUHU"];
        $t5 = $row["RELAY"];
        $t6 = $row["SUBMIT"];
        
        echo '<tr> 
                <td><b>' . $t1 . '</b></td> 
                <td><b>' . $t2 . '</b></td>
                <td><b>' . $t3 . '</b></td> 
                <td><b>' . $t4 . '</b></td> 
                <td><b>' . $t5 . '</b></td> 
                <td><b>' . $t6 . '</b></td>   
              </tr>';
    }       
    $result->free();
}

$conn->close();
?> 
</body>
</html>