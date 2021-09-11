<?php

$username = "admin";
$pass = "adminhensem";
$host = "localhost";
$db_name = "makex";

$con = mysqli_connect ($host, $username, $pass);


$db = mysqli_select_db ($con, $db_name);

echo "test";

$sql_insert = "INSERT INTO lrt_jb_log (NAME , STATUS) VALUES ('".$_GET["name"]."','".$_GET["stat"]."')";

echo "test";

if(mysqli_query($con,$sql_insert))
{
    echo "Done";
    mysqli_close($con);
}
else
{
    echo "error is ".mysqli_error($con );
}

?>