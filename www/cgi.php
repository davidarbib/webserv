<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>WELCOME PAGE !</title>
</head>
<body>
<style>
        body
        {
            background-color: rgb(144,230,229);
            text-align: center;
            padding-top: 10%;
        }

        h1
        {
            font-weight: bold;
            font-size: 5rem;
            color: rgb(78, 185, 185);
            font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif;
        }

        p
        {
            font-size: 1.3rem;
            color: rgb(17,17,17);
        }

        a
        {
            color: rgb(78, 185, 185);
            transition: 0.6s;
        }

        a:hover
        {
            color: rgb(255, 255, 255);
            text-decoration: none;
        }

        input
        {
            font-size: 3rem;
            background-color: rgb(144,230,229);
        }

        button
        {
            font-size: 3rem;
            margin-right: 5rem;
            background-color: rgb(255, 255, 255);
            border: 5px solid rgb(78, 185, 185);
            border-radius: 5px;
            display: inline-block;
        }

        button a
        {
            text-decoration: none;
        }

        button:hover
        {
            background-color: rgb(78, 185, 185);
            transition: 0.6s;
        }

        button a:hover
        {
            color: rgb(255, 255, 255);
        }
    </style>
		<h1><?php echo 'Hello ' . htmlspecialchars($_GET["name"]) . '!'; ?></h1>
</body>
</html>