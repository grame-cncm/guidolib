<?php
  $resizepagetomusc = $_POST["resizepagetomusic"];
  if ($resizepagetomusic == '')
  {
    $resizepagetomusic = 'false';
  }
  $gmn = $_POST["gmn"];
  //$gmn = preg_replace('/\%.*/', "", $gmn); // percentage sign
  //$gmn = preg_replace('(/\(\*..*\*\)/', "", $gmn); // something I don't understand
  //$gmn = preg_replace('/\n/', "   ", $gmn); // newline
  //$gmn = preg_replace('/#/', "%23", $gmn); // hash
  //$gmn = preg_replace('/ +/', " ", $gmn); // something I don't understand
  //$gmn = preg_replace('/&/',"%26", $gmn); // ampersand
  $content = 'data=' . $gmn . '';

  $params = array(
    'http' => array( 
      'method' => 'POST', 
      'content' => $content
  )); 
  $ctx = stream_context_create($params); 
  $fp = @fopen('http://guido.grame.fr:8000/', 'rb', false, $ctx); 
  if (!$fp) { 
    throw new Exception("Couldn't connect to the server."); 
  } 
  $response = @stream_get_contents($fp);
  if ($response === false) { 
    throw new Exception("Didn't get a response from the server.");
  }
  $json = json_decode($response, true);
  // we check to see if we have the correct key...
  if (!array_key_exists("ID", $json)) {
    header('Content-Type: application/json');
    echo $response;
  }
  else {
    $identifier = $json["ID"];
    $params = array(
      'http' => array( 
        'method' => 'GET'
    )); 
    $ctx = stream_context_create($params); 
    $fp = @fopen('http://guido.grame.fr:8000/' . $identifier , 'rb', false, $ctx);
    if (!$fp) { 
      throw new Exception("Couldn't connect to the server."); 
    } 
    $response = @stream_get_contents($fp);
    $im = imagecreatefromstring($response);
    if ($im !== false) {
      header('Content-Type: image/png');
      imagepng($im);
      imagedestroy($im);
    }
    else {
      throw new Exception("Didn't get an image from the server.");
    }
  }
?>
