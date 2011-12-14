<?php
/**
 * @file
 *
 * Single review template.
 */
?>

<div class="voxbReview">
  <?php print t('Written by'); ?> <strong><?php echo htmlspecialchars($data['author']); ?></strong>
  <div class="reviewContent"><em><?php echo htmlspecialchars($data['review']); ?></em></div>
</div>
