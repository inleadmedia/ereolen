<?php
/**
 * @file
 */
?>
<div class="feed-and-compare-front clear-block">

  <?php if ($items['status'] == 'empty') { ?>
    <div class="feed_and_compare_empty" style="border:1px dashed red;margin:0 10px 10px 0;">
      <b>nothing found</b>
    </div>
  <?php } ?>

  <?php if ($items['status'] == 'error') { ?>
    <div class="feed_and_compare_error" style="border:1px dashed red;margin:0 10px 10px 0;">
      <b>error:</b> <?php echo $items['message']; ?>
    </div>
  <?php } ?>

  <?php if (($items['status'] == 'ok') || ($items['status'] == 'notfull')) { ?>
    <?php $i=0; ?>
    <?php foreach ($items['data'] as $key => $item) { ?>
      <?php if (is_numeric($key)) { ?>
        <div class="feed_and_compare_front_item" style="border:1px dashed red;width:48%;float:left;margin:0 10px 10px 0;">
          <div class="feed_and_compare_front_item_image">
            <b>image:</b> <?php echo theme_image($item['image_url'], '', '', array('width' => '100px'), FALSE); ?><br />
          </div>
          <div class="feed_and_compare_front_item_descr">
            <div class="feed_and_compare_item_isbn">
              <b>ISBN:</b> <?php echo $item['isbn']; ?><br />
            </div>
            <div class="feed_and_compare_front_item_title">
              <b>title:</b> <?php echo $item['title']; ?><br />
            </div>
            <div class="feed_and_compare_front_item_author">
              <b>author:</b> <?php echo $item['author']; ?><br />
            </div>
            <div class="feed_and_compare_front_item_description">
              <b>teaser:</b> <?php echo $item['abstract']; ?><br />
            </div>
          </div>
        </div>
        <?php $i++; ?>
      <?php } ?>
      <?php
        if ($i == variable_get('litteratursiden_feed_compare_items_on_front', '2')) {
          break;
        }
      ?>
    <?php } ?>
  <?php } ?>

</div>
