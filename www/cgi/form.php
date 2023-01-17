<?php
// phpinfo();
// print_r($_GET);
// print_r($_POST);
if (isset($_GET['submit']) && $_SERVER["REQUEST_METHOD"] == "GET") {
    echo '<h3>Form GET Method</h3>';
    $lastname = $_GET['lastname'];
    $firstname = $_GET['firstname'];
    echo 'Your name is '. $lastname . ' ' . $firstname;
    exit;
}
else if (isset($_POST['submit']) && $_SERVER["REQUEST_METHOD"] == "POST"){
    echo '<h3>Form POST Method</h3>';
    $lastname = $_POST['lastname'];
    $firstname = $_POST['firstname'];
    echo 'Your name is '. $lastname . ' ' . $firstname;
    exit;
}
else
    echo "NO SUBMIT";
?>