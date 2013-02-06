<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="da" lang="da" dir="ltr">
  <head>
    <script type="text/javascript" src="//reader.pubhub.dk/scripts/reader-1.1.4/reader.js"></script>
    <script type="text/javascript">
      var reader = new Reader ({
        elementId: 'elib-reader',
        orderId: '<?php print $vars['order_number']; ?>',
        sessionKeyUrl: '/<?php print $vars['session_url']; ?>'
      });
      reader.Display();
    </script>
  </head>
  <body>
    <div id="elib-reader"></div>
  </body>
</html>
