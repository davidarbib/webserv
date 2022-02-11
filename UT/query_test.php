<?php
	$body = file_get_contents("php://input");
	$key = 'meal';
	echo $key . ' : ' . $_GET[$key] . "\n";
	echo 'len = ' . getenv("CONTENT_LENGTH") . "\n";
	echo 'body : ' . $body . "\n";
?>
