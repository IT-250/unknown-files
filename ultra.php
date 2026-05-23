<?php
$conn = new mysqli("localhost","root","","ultra_db");
$dist = $_GET['distance'];
$stat = $_GET['status'];
$conn->query("INSERT INTO `distance_tb` (`distance`, `status`) VALUES ('$dist','$stat')");
$result = $conn->query("SELECT * FROM `distance_tb` ORDER BY id DESC");
?>


<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>MONITOR</title>
</head>
<body>
<table border="1">
<tr>
<th>id</th>
<th>distance</th>
<th>status</th>
<th>timestamp</th>
</tr>
<?php
while($row = $result->fetch_assoc()){
?>
<tr>
<td><?php echo $row['id']; ?></td>
<td><?php echo $row['distance']; ?></td>
<td><?php echo $row['status']; ?></td>
<td><?php echo $row['timestamp']; ?></td>
</tr>
<?php
}
?>
</table>  
</body>
</html>