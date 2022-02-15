<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Document</title>
</head>
<body>
	<h1>Hello World </h1>
	<?php
		$body = file_get_contents("php://input");
		$key = 'meal';
		echo $key . ' : ' . $_GET[$key] . "\n";
		echo 'len = ' . getenv("CONTENT_LENGTH") . "\n";
		$html_page;
		// echo 'body : ' . $body . "\n";
	?>
</body>
</html>