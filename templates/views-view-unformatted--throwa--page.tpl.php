<?php

/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */

 // we want this report to be in the same order as the census tabs See explicit Order we want below
 // $orderWeWant = array(
 //   'Education Programs',
 //   'Entrepreneurship and Business Development Programs',
 //   'Housing and Community Development Programs',
 //   'Workforce Development Programs',
 //   'Civic Engagement',
 // );
?>
<?php if (!empty($title)): ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>

<!-- It looks to me like the only thing wrong with the order is that civic engagement is first and it should be last, this just makes it last -->
<?php $civic = array_shift($rows); $rows[] = $civic; ?>
<!-- end of custom code -->
<?php foreach ($rows as $id => $row): ?>
  <div<?php if ($classes_array[$id]) { print ' class="' . $classes_array[$id] .'"';  } ?>>
    <?php print $row; ?>
  </div>
<?php endforeach; ?>
