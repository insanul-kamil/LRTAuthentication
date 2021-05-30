<?php

    include ('connection.php');

    $result = mysqli_query($con,'SELECT * FROM user ORDER BY id');

    while($row = mysqli_fetch_array($result))
    {      
        echo "<tr>";
        echo "<td>" . $row['id'] . "</td>";
        echo "<td>" . $row['time'] . "</td>";
        echo "<td>" . $row['value'] . "</td>";
        echo "</tr>";
    
    }
    
    // Close the connection   
    mysqli_close($con);

?>