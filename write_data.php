<?php

    include ('connection.php');

    $sql_insert = "INSERT INTO lrt_jb_log (NAME) VALUES ('".$_GET["name"]."')";

    if(mysqli_query($con,$sql_insert))
    {
        echo "Done";
        mysqli_close($con);
    }
    else
    {
        echo "error is ".mysqli_error($con);
    }

?>