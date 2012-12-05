<?php
/**
 * @file
 *
 */
?>
<div>
  <div class="left">
    <div class="picture">
      <a href=""><img title="" alt="" src="<?php echo $product->getCover('65_x'); ?>" /></a>
    </div>
  </div>
  <div class="right">
    <h3 class="title"><?php echo $title; ?></h3>
    <div class="author"><?php echo $authors; ?></div>
    <?php echo $actions; ?>
  </div>
</div>