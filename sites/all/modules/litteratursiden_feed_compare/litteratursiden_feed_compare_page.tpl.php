<?php foreach ($objects as $object) { ?>
  <div id="feed_and_compare_item" style="border:1px dashed red;margin-bottom:10px;">
    ISBN: <?php echo $object->record['dc:identifier']['dkdcplus:ISBN'][0]; ?><br>
    title: <?php echo $object->title; ?><br>
    author: <?php echo $object->creators_string; ?><br>
  </div>
<?php } ?>
