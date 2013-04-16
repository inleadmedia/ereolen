<?php
/**
 * @file
 * Template for author portrait block.
 */
?>
<div class="ebog-author-portrait">
  <div class="ebog-author-portrait-image">
    <?php echo theme_image($conf['image'], '' , '', array(), FALSE); ?>
  </div>
  <div class="ebog-author-portrait-descr">
    <h3 class="title"><?php echo $conf['portrait']['name']; ?></h2>
    <p class="teaser"><?php echo $conf['portrait']['teaser']; ?></p>
    <p class="litteratursiden-link"><?php echo l(theme_image('sites/all/themes/ebog/images/litteratursiden.png', '' , '', array(), FALSE), $conf['portrait']['url'], array('html' => TRUE, 'absolute' => TRUE)); ?></p>
  </div>
  <div class="clear"></div>
</div>
<div class="bottom-bar">
  <div class="see-more"><?php echo l(t('Read more at litteratursiden.dk...'), $conf['portrait']['more']); ?></div>
</div>
