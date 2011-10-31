<?php
/**
 * @file
 *
 * Template for info messages.
 */
?>
<div class="ebog-download-info">
  <p><?php echo $data['message']; ?></p>
  <?php if (isset($data['link'])) { ?>
  <p><a class="ebog-dlink" class="down_link" href="<?php echo $data['link']; ?>"><?php print t('Download');?></a></p>
  <?php } ?>
</div>
