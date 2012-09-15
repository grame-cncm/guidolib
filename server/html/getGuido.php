<?php
  $contentstr = 'get=' . $_POST["getmenu"];
  if ($_POST["getmenu"] == 'staffmap')
  {
    $contentstr = $contentstr . '&staff=' . $_POST["staff"];
  }
  else if ($_POST["getmenu"] == 'voicemap')
  {
    $contentstr = $contentstr . '&voice=' . $_POST["voice"];
  }
  else if ($_POST["getmenu"] == 'point')
  {
    $contentstr = $contentstr . '&x=' . $_POST["x"] . '&y=' . $_POST["y"]
    . '&map=' . $_POST["maptype"];
    if ($_POST["maptype"] == 'staff')
    {
      $contentstr = $contentstr . '&staff=' . $_POST["staff"];
    }
    else if ($_POST["maptype"] == 'voice')
    {
      $contentstr = $contentstr . '&voice=' . $_POST["voice"];
    }    
  }
  $response = http_get("http://guido.grame.fr:8000/?" . $contentstr);
  if ($response === false) { 
    throw new Exception("Didn't get a response from the server.");     
  }
   
  header('Content-Type: application/json');
  echo $response;
?>
