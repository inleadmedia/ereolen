<?php
/**
 * @file
 *
 */
?>
<div class="display-book biglist">
  <div class="left">
    <div class="picture">
      <a href=""><img title="" alt="" src="<?php echo $loan->product->getCover('65_x'); ?>" /></a>
    </div>
  </div>
  <div class="right">
    <h3 class="title"><?php echo $title ?></h3>
    <div class="author"><?php echo $authors ?></div>
    <div class="loanperiod short"><?php echo $loan->loanExpiresIn();?></div>
  </div>
</div>