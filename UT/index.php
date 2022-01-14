<?php
	$body = file_get_contents("php://input");
	echo 'len = ' . getenv("CONTENT_LENGTH") . "\n";
	echo 'body : ' . $body . "\n";
?>
