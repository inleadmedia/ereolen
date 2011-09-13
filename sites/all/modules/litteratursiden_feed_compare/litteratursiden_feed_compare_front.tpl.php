<div id="feed_and_compare_front">
  <div id="feed_and_compare_front_right" style="float:left;width:49%;border:1px dashed red;">
    ISBN: <?php echo $objects[0]->record['dc:identifier']['dkdcplus:ISBN'][0]; ?><br>
    title: <?php echo $objects[0]->title; ?><br>
    author: <?php echo $objects[0]->creators_string; ?><br>
  </div>
  <div id="feed_and_compare_front_right" style="float:right;width:49%;border:1px dashed red;">
    ISBN: <?php echo $objects[1]->record['dc:identifier']['dkdcplus:ISBN'][0]; ?><br>
    title: <?php echo $objects[1]->title; ?><br>
    author: <?php echo $objects[1]->creators_string; ?><br>
  </div>
</div>
