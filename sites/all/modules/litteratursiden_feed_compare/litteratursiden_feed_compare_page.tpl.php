<?php foreach ($items as $item) { ?>
  <div id="feed_and_compare_item" style="border:1px dashed red;margin-bottom:10px;">
    ISBN: <?php echo $item['record']['dc:identifier']['dkdcplus:ISBN'][0]; ?><br>
    title: <?php echo $item['title']; ?><br>
    author: <?php echo $item['creators_string']; ?><br>
  </div>
<?php } ?>
